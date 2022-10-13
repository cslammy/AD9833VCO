#include "pico/stdlib.h"
#include <stdint.h>
#include <stdio.h>

uint16_t AD9833_CNTL_SPI[1];  //global buffer for AD9833 control SPI writes
uint16_t AD9833_FREQ_SPI[2];  //global buffer for AD9833 freq SPI writes
uint16_t AD9833_PHASE_SPI[1]; //global buffer for AD9833 phase SPI writes
uint16_t AD9833_init_data[6];  //initialize AD9833 buffer after resetstat
#include "ad9833.h"
#include "hex2bin.h"
#include "spiRP2040.h"
#include "UART.h"
#include "MCP3201.h"
#include "freq2array.c"

/*
IMPORTANT
project uses SEEEDRP2040,
https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html 
so some pins are different vs.
Pico Dev board
*/



const uint LED_PIN = 16; // green LED on SEEED2040




int main() 
{

uint16_t MCPvalue = 0;

//initialize UART

stdio_init_all();
 
gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);

//initialize SPI
init_spi_master();
sleep_ms(500);

spi_mode(0);
sleep_ms(1000);

//initialize device--power up reset
AD9833_powerup_reset();
SPI_TransferTx16_variable_num_words(spi0, AD9833_CNTL_SPI, 1);
sleep_ms(20);

//initialize device--initial freq and phase
AD9833_init();
SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2); // initial FREQ0
SPI_TransferTx16_variable_num_words(spi0, AD9833_PHASE_SPI, 1); // initial FREQ0
SPI_TransferTx16_variable_num_words(spi0, AD9833_CNTL_SPI, 1); // FREQ0, phase0-0, run mode
sleep_ms(1000);


//loop dukie here
    while (2 > 0) 
    {
    READ_UART_BUFFER; 
    printf("%s\n",uart_buffer); 
    
    MCPvalue = MCP3201read();
    printf("value of MCP is %d \n",MCPvalue);
    sleep_us (10);
    
 
/*
   //trap buffer....serial commands to present to AD9833 code

set your PC to 8N1, no hw flow control, 115200 baud, no sw flow control
enter your string below without quotes in the termianl program.
the keywords will launch processes on the AD9833--see the results on a scope.
*/ 

/* summary of AD9833 library test routines provided in this main.c file.

REBOOT  reboots the dev board allowing a UF2 upload.
LED_OFF  turns onboard LED off. assumes PICO dev board  
LED_ON  turns  onboard LED on.  assumes PICO dev board

TRI  switch to triangle wave
SQUARE ditto, square wave
SINE  ditto, sine wave

 
F1  sanity check of freq0 register to output
F2  sanity check of freq1 register to output

FRQxxxxx loads freq reg0 with freq xxxxxx.  xxxxx must be 5 digits, so 100hz is 00100
F1Qxxxxx ditto for freq reg1
 
REG  print out current freq, control, phase registers as seen by the PC

MSB  increment F0 MSB only causing coarse frequency change
LSB  ditto for LSB, fine frequency change.

*/
//next 3 ifs for test and utility
       if (UART_READ("REBOOT"))
        {
        reset_usb_boot(0,0);
        } 

        if (UART_READ("LED_ON"))
         
        {
        //LEDs on SEEED 2040 are wired backwards!  
        //0 = on, 1 = off
        //Default state of LED is ON
        printf("turning on LED...\n");
        gpio_put(LED_PIN,0);
        }
    
    
    if (UART_READ("LED_OFF"))

        {
        printf("turning off LED...\n");
        gpio_put(LED_PIN,1);
        }

    //rest is for testing the library.....
    if (UART_READ("TRI")) //gen tri wave
        {
            AD9833_Tri();
            CNTL_SEND; //macro to send via SPI the control uint16_t
            //printf("Sending control 0x%x \n",AD9833_CNTL_SPI[0]);  
        }

        if (UART_READ("SINE")) //gen sine wave
        {
            AD9833_Sine();
            CNTL_SEND;          
        }

        if (UART_READ("SQUARE")) //gen square wave
        {
            AD9833_Sine(); //you can't switch from tri directly to square
            CNTL_SEND;  
            AD9833_square(); 
            CNTL_SEND;  
        }

 
        if (UART_READ("F1"))
        {
            AD9833_freq0_to_output();
            CNTL_SEND;
            //general frequency sanity check
            AD9833_FREQ_SPI[0]=0x50C7;
            AD9833_FREQ_SPI[1]=0x400B;
            SPI_TransferTx16_variable_num_words(spi0,AD9833_FREQ_SPI, 2);

        }
        
         //general frequency sanity check
        if (UART_READ("F2"))
        {
            AD9833_freq0_to_output();
            CNTL_SEND;
            AD9833_FREQ_SPI[0]=0x40C7;
            AD9833_FREQ_SPI[1]=0x40FF;
            SPI_TransferTx16_variable_num_words(spi0,AD9833_FREQ_SPI, 2);
        }

        if (UART_READ("S1"))
        {
            //switch to Freq reg 1

            AD9833_freq1_to_output();
            CNTL_SEND;

        }
        
        if (UART_READ("S0"))
        {
            //switch to Freq reg 0

            AD9833_freq0_to_output();
            CNTL_SEND;

        }


        if (UART_READ("REG"))
        {
            //read current array values
 
            printf("Control reg value is: %x \n",AD9833_CNTL_SPI[0]);
            

            printf("freq reg value is: LSB %x  and MSB %x \n",AD9833_FREQ_SPI[0],AD9833_FREQ_SPI[1]);
            printf("phase reg value is: %x \n",AD9833_PHASE_SPI[0]);
            
        }
         //load FRQ1 register with freq. value
        char f[4] = "FRQ"; // this has to be [+1] greater than # of chars--but why scotty why?
        
        if (compare_strings(uart_buffer,f,3)) // compare first 3 chars
        {
            AD9833_freq0_to_output();  
            CNTL_SEND;
            uint32_t rez = 0;
            //printf("YOU are in FRQ true....buffer is: %s  \n",uart_buffer);
            //printf("f is: %s \n",f );
            rez = dumpleft_conv2dec(uart_buffer, 3, 5);
            
            AD9833_freq_load(0,rez);
            SPI_TransferTx16_variable_num_words(spi0,AD9833_FREQ_SPI, 2);
        }

        //load FRQ1 register with freq. value
        char fa[4] = "F1Q"; // this has to be [+1] greater than # of chars--but why scotty why?
        
        if (compare_strings(uart_buffer,fa,3)) // compare first 3 chars
        {
            uint32_t rez = 0;
            //printf("YOU are in FRQ true....buffer is: %s  \n",uart_buffer);
            //printf("f is: %s \n",f );
            rez = dumpleft_conv2dec(uart_buffer, 3, 5);
            
            for (uint8_t x = 1; x++; x<200)
            {
            AD9833_freq_load(1,rez);
            SPI_TransferTx16_variable_num_words(spi0,AD9833_FREQ_SPI, 2);
            
    
            AD9833_release_reset();
            AD9833_Load_MSB_LSB();
            AD9833_freq1_to_output();
             
            CNTL_SEND;
            printf("control reg is: 0x%x \n",AD9833_CNTL_SPI[0]);
            sleep_ms(10);
            }
        }


    } //end while
return 0;
} //end main
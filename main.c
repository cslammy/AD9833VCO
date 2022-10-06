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



//initialize UART

stdio_init_all();
 
gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);

//initialize SPI
init_spi_master();
sleep_ms(500);

spi_mode(2);
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
    

    //trap buffer....serial commands to present to AD9833 code
/*
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

SQUAREHALF switch to square wave--1/2 frequency of chosen retister
FSK uses FSELECT to quickly switch between freq in reg 0 and 1
PHASE  simple phase mod test

STAIR  sample and hold of triagle wave by freezing and unfreezing DAC
F1  sanity check of freq0 register to output
F2  sanity check of freq1 register to output

FRQxxxxx loads freq reg0 with freq xxxxxx.  xxxxx must be 5 digits, so 100hz is 00100
F1Qxxxxx ditto for freq reg1

S0   select Freq req0 to output
S1   select Freq reg1 to output
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

        if (UART_READ("SQUAREHALF")) //gen square wave, half freq
        {
            AD9833_Sine();
            CNTL_SEND; 
            AD9833_square_halffreq();
            CNTL_SEND;   
        }

 
        if (UART_READ("LSB")) 
        {
        AD9833_freq0_to_output;
        AD9833_Load_MSB_only;
        CNTL_SEND;

            for (uint32_t xx = 0; xx <= 4000; xx= xx + 100)
            {
                AD9833_freq_load(0,xx);
                SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);
                sleep_ms(300);
            }
        }
        
        
        
        if (UART_READ("MSB")) 
        {
        AD9833_freq0_to_output;
        AD9833_Load_LSB_only;
        CNTL_SEND;

            for (uint32_t xx = 0; xx <= 4000000; xx = xx + 100000){
                AD9833_freq_load(0,xx);
                SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);
                sleep_ms(300);
            }
        }

        if (UART_READ("FSK")) //freq change by loading up freq1 and 0 regs then switching between them
        
        {

            //load freq registers

            AD9833_freq_load(0,12200);
            SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);

            AD9833_freq_load(1,1000);  
            SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);

            
            
            printf("Control reg before freq1 change  is: %x \n",AD9833_CNTL_SPI[0]);
             

            for (uint16_t i = 0; i<300; i++)
            {
            AD9833_freq1_to_output();
             
            
            printf("Control reg after freq1 change  is: %x \n",AD9833_CNTL_SPI[0]); 
            SPI_TransferTx16_variable_num_words(spi0,AD9833_CNTL_SPI, 1);
            sleep_ms(10);
            
            AD9833_freq0_to_output();
             
            
            printf("Control reg after freq1 change  is: %x \n",AD9833_CNTL_SPI[0]); 
            SPI_TransferTx16_variable_num_words(spi0,AD9833_CNTL_SPI, 1);
            sleep_ms(10);

           

            }
        }

                if (UART_READ("FSKLONG")) //freq change by loading up freq1 and 0 regs then switching between them
        
        {

            /*SAME test as FSK but with 10x longer loop.  
            /this is here to see if SPI in general is working
            more time to probe pins
            */

            //load freq registers

            AD9833_freq_load(0,12200);
            SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);

            AD9833_freq_load(1,1000);  
            SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);

            
            
            printf("Control reg before freq1 change  is: %x \n",AD9833_CNTL_SPI[0]);
             

            for (uint16_t i = 0; i<3000; i++)
            {
            AD9833_freq1_to_output();
             
            
            printf("FSKLONG routine. Control reg after freq1 change  is: %x \n",AD9833_CNTL_SPI[0]); 
            SPI_TransferTx16_variable_num_words(spi0,AD9833_CNTL_SPI, 1);
            sleep_ms(10);
            
            AD9833_freq0_to_output();
             
            
            printf("FRKLONG routine. Control reg after freq1 change  is: %x \n",AD9833_CNTL_SPI[0]); 
            SPI_TransferTx16_variable_num_words(spi0,AD9833_CNTL_SPI, 1);
            sleep_ms(10);

           

            }
        }
        if (UART_READ("PHASE")) //test phase stuff w phase modulation
        //"works" but need an algo to accurately move phase.
        {
            AD9833_freq_load(0,100); //slow signal
            SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);
            AD9833_Sine();
            SPI_TransferTx16_variable_num_words(spi0,AD9833_CNTL_SPI, 1);
            


            //reg 1 phase 2 out
            AD9833_phase(0,0);
            SPI_TransferTx16_variable_num_words(spi0,AD9833_PHASE_SPI, 1);
            AD9833_phase(1,1000);
            SPI_TransferTx16_variable_num_words(spi0,AD9833_PHASE_SPI, 1);
            
            


            for (uint16_t i = 0; i < 20;i++)
            {
            AD9833_phase1_to_output();
            CNTL_SEND;
            printf("control reg 1 is: 0x%x \n",AD9833_CNTL_SPI[0]);

            sleep_ms(500);
            AD9833_phase0_to_output();
            CNTL_SEND;
            printf("control reg is: 0x%x \n",AD9833_CNTL_SPI[0]);
            sleep_ms(500);
            }
            
        }
               
        if (UART_READ("STAIR")) //test DAC on and off stuff
        {
           AD9833_freq0_to_output();
            
           AD9833_Tri();
           CNTL_SEND;
           AD9833_freq_load(0,60); //slow signal
           SPI_TransferTx16_variable_num_words(spi0, AD9833_FREQ_SPI, 2);
           for (uint16_t i = 1; i < 300; i++)
           {
            AD9833_DAC_freeze();
            CNTL_SEND;
            sleep_ms(10);
            AD9833_DAC_unfreeze();
            CNTL_SEND;
            sleep_ms(10);
           } 


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

        /*we capture FRQ to know we are testing frequency algo.
        you must enter the FRQ you want to test followed 5 digits
        E.G.:
        FRQ01000    1K
        FRQ00100    100hz
        FRQ20000    20K   etc.
        */

        //load FRQ0 register. If cntl is set to favor F0 this will change output freq immediately
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
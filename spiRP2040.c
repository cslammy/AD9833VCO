/*
 * spiRP2040.c
 *

port of Spi3.c and h for AVR.

 */ 



#include "spiRP2040.h"
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SPI_BITS  16  // bits for each SPI xfer (usually 8 or 16)
#define SPI_SPEED 1000000
//set speeds

//for audio use 60M (max)
//int spi_speed = 60000000;
//for pulseview use about 500K so you can see what you're doing
int spi_speed = SPI_SPEED;

//set bits per transfer
uint8_t bits_per_transfer = SPI_BITS;
//buffers to hold our data




uint16_t data[2]; //16 bits
uint8_t data8[1]; //8 bits
uint8_t data8ret[1]; // bits for read return


    //set pins
const uint cs_pin = 5;
const uint sck_pin = 2;
const uint mosi_pin = 3;
const uint miso_pin = 4;




void init_spi_master(void)
{
	
	
	 //use spi0
	spi_inst_t *spi=spi0; 
	gpio_init(cs_pin); 
	gpio_set_dir(cs_pin, GPIO_OUT);  // set cs_pin to output
    gpio_put(cs_pin,1); // set cs_pin initially to high
    spi_init(spi,spi_speed);

gpio_set_function(sck_pin, GPIO_FUNC_SPI);
gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
gpio_set_function(miso_pin, GPIO_FUNC_SPI);



}


void spi_mode(uint8_t mode)
{
if (mode == 1)
	{
     spi_set_format(spi0,SPI_BITS,0,1,SPI_MSB_FIRST);
	 printf("you are using SPImode 1 \n");
	 
	}
if (mode == 2)
	{
     spi_set_format(spi0,SPI_BITS,1,0,SPI_MSB_FIRST);
	 printf("you are using SPImode 2 \n");
	}
if (mode == 3)
	{
     spi_set_format(spi0,SPI_BITS,1,1,SPI_MSB_FIRST);
	 printf("you are using SPImode 3 \n");
	}
if (mode == 0)
		{
        spi_set_format(spi0,SPI_BITS,0,0,SPI_MSB_FIRST); // mode 0
		printf("you are using SPImode 0 \n");
		}
	
}

void SPI_TransferTx8(spi_inst_t *spi, const uint8_t a) // single 8bit write 
 {
    
	  data8[0] = a;
	  gpio_put (cs_pin,0);	  
	  spi_write_blocking(spi,data8, 1);
      gpio_put (cs_pin,1);

 }
 

void SPI_TransferTx8_variable_num_words(spi_inst_t *spi, const uint8_t a[0], uint8_t numwords) // 8bit write, cs down; variable # of bytes before CSup 
 {
    
	  

	  gpio_put (cs_pin,0);	  
	  spi_write_blocking(spi,a, numwords);
      gpio_put (cs_pin,1);

 }

void SPI_TransferTx16_variable_num_words(spi_inst_t *spi, const uint16_t a[0], uint8_t numwords) // csdown, 16 bit writes, cs up; variable # of bytes before CSup 
 {

	  gpio_put (cs_pin,0);	  
	  spi_write16_blocking(spi,a, numwords);
      gpio_put (cs_pin,1);

 }


void SPI_TransferTx16(spi_inst_t *spi, const uint8_t a,  const uint8_t b)  // 2 8bit sequential writes
 {
	 
	
	  data8[0] = a;
	  //MSB
 
	  gpio_put (cs_pin,0);
	  spi_write_blocking(spi,data8, 1);
      gpio_put (cs_pin,1);
	 

    //LSB
	  data8[0] = b;
	
	  gpio_put (cs_pin,0);
	  spi_write_blocking(spi,data8, 1);
      gpio_put (cs_pin,1);

 }

 
 
void  SPI_TransferTx16_SingleCS(spi_inst_t *spi, const uint16_t data) // cs low, 2 bytes, cs high)
 {
   
	  gpio_put (cs_pin,0);
	  
	  //uint16_t data16 = data; // create 16 bit buffer
      //data16 = data;
      spi_write16_blocking(spi,&data, 1);
	 
	  gpio_put (cs_pin,1);
 }
 
uint8_t SPI_TransferTxRx8(spi_inst_t *spi, const uint8_t data8) // CS down, 8 bits, CS UP 
{
      
	  
	  gpio_put (cs_pin,0);	  
	   
      spi_read_blocking(spi, data8, data8ret, 1);
	  gpio_put (cs_pin,1);
	  return data8ret[0];

}

uint16_t SPI_TransferTxRx16_SingleCS(spi_inst_t *spi, const uint16_t data_in) // cs low, 2 bytes to send to SPI IC, cs high)
{
	  gpio_put (cs_pin,0);
	  
      uint16_t data16ret = 0;
      
      spi_read16_blocking(spi,data_in, &data16ret, 1);
	
	  gpio_put (cs_pin,1);
	  return data16ret;

}
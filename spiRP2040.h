/*
port of SPI library, for RP2040 MCU
	
note! be sure to include

hardware_spi

in CMakeLists.txt "target_link_libraries"
 */ 


#ifndef SPI_H_
#define SPI_H_


#include "stdint.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"


/* declarations */
void init_spi_master(void);

//mode is 0,1,2, or 3, SPI mode. 0 is default
void spi_mode(uint8_t mode);

//send a byte to a SPI IC for write
// reminder: send 8 bits to a SPI peripheral--CS down, 8 bits, CS UP
// PARAMETERS: *spi is spi0 or spi1; data8 is unsigned byte to write to IC
void SPI_TransferTx8(spi_inst_t *spi, uint8_t data8); 

//send 16 bits via 2 bytes with a CS before and after each byte. 
//reminder: be sure to set #define SPI_BITS to 8 in spiRP2040.c 
//PARAMETERS: *spi is spi0 or spi1  a is MSB,  b is LSB
void SPI_TransferTx16(spi_inst_t *spi, uint8_t a, uint8_t b);  

 
//send 16 bits as single word for write
//reminder: be sure to set #define SPI_BITS to 16 in spiRP2040.c 
//PARAMETERS: *spi is spi0 or spi1 datain is uint16_t 
void SPI_TransferTx16_SingleCS(spi_inst_t *spi, uint16_t data_in); 

//send numwords number of bytes via spi for write
//reminder: set #define SPI_BITS to 8 in spiRP2040.c 
//PARAMETERS: spi* is spi0 or spi1  a[0] is array with number or words in numwords.
void SPI_TransferTx8_variable_num_words(spi_inst_t *spi, const uint8_t a[0], uint8_t numwords);

//send variable number of 16 bit words via spi for write
//reminder: set #define SPI_BITS to 16 in spiRP2040.c 
//PARAMETERS: spi* is spi0 or spi1  a[0] is array with number of words with same number of members as uint8_t variable numwords.
void SPI_TransferTx16_variable_num_words(spi_inst_t *spi, const uint16_t a[0], uint8_t numwords);// csdown, 16 bit writes, cs up; variable # of bytes before CSup 

//send 8 bits to a SPI peripheral and receive 8 bits from peripheral
//reminder: set #define SPI_BITS to 8 in spiRP2040.c 
//PARAMETERS: spi* is spi0 or spi1  data8 is a byte.  returns a single uint8_t
uint8_t SPI_TransferTxRx8(spi_inst_t *spi, uint8_t data8); 

//send 16 bits to a SPI peripheral and receive 16 bits from peripheral--cs low, uint16_t, cs high);
//reminder: set #define SPI_BITS to 16 in spiRP2040.c 
//PARAMETERS: spi* is spi0 or spi1  data_in is 16 bits.  returns a single uint8_t
uint16_t SPI_TransferTxRx16_SingleCS(spi_inst_t *spi, uint16_t data_in); 

//send 16 bits to a SPI peripheral and receive 16 bits from peripheral--cs low, uint16_t, cs high);
//reminder: set #define SPI_BITS to 16 in spiRP2040.c 
//PARAMETERS: spi* is spi0 or spi1  data_in is 16 bits.  returns a single uint8_t
//uses alternate CS_pin, "cs_pin2"
uint16_t SPI_TransferTxRx16_SingleCS_CSpin2(spi_inst_t *spi, const uint16_t data_in);

#endif /* SPI_H_ */
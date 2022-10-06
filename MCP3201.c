 
#include <stdint.h>
#include "MCP3201.h"
#include "stdio.h"
#include "spiRP2040.h"

//variables needed for this IC
static uint16_t inputword = 0xFFFF; // can be anything 16 bits
uint16_t mcp3201result = 0;
static uint16_t rawdataread = 0;  // result of initial read from MCP3201, before bitmoves

 // SEEED2040, need to use pin1 for CS for MCP3201


    
    uint16_t MCP3201read(void)
    {
        //reset initial values

        //change SPI mode to 0. 

    //spi_mode(0);
	//printf("you are using SPImode 0 \n");   
    

	  
	  //read the data for the ADC
    rawdataread = SPI_TransferTxRx16_SingleCS_CSpin2(spi0, inputword) ;


    
    rawdataread = rawdataread << 3;
    mcp3201result = rawdataread >> 4;
    
    // set SPI back to mode 2 (needed for AD9833)
    
     
    return mcp3201result;
    }

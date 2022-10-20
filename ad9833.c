#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "ad9833.h"

 
//put this global dec in .h file for IC

//structure defs--this goes in AD9833.h





float MCLK = 25000000; //crystal on BoB is this speed

/* syntax for pointer to struct is strange!
 struct bytes1 *BytesPtr, ByteMe;
    BytesPtr = &ByteMe;
*/
//create pointer to struct
struct AD9833_C *AD9833_Control, Byteme0;


//functions, these go in AD9833.c

void AD9833_init(void) //uint16_t's for initialization
//see AN-1070 app note: 
//https://www.analog.com/media/en/technical-documentation/application-notes/an-1070.pdf
// reset, like 0x2100, needed before this
//however, 0x0100 for initial power on is needed not 0x2100 as in AN1070
{

//load buffers with starting data

AD9833_FREQ_SPI[0] = 0x50C7;
AD9833_FREQ_SPI[1] = 0x4000;
AD9833_PHASE_SPI[0] = 0xC000;
AD9833_CNTL_SPI[0] = 0x2000;
 
}

void control_value_sum(void)
//NOT USED
{
            AD9833_Control = &Byteme0;
            AD9833_CNTL_SPI[0] =
            (AD9833_Control ->  B28 << 13) +
            (AD9833_Control ->  HLB << 12)+
            (AD9833_Control ->  FSELECT << 11)+
            (AD9833_Control ->  PSELECT << 10)+
            (AD9833_Control ->  reset << 8) +
            (AD9833_Control ->  SLEEP << 7) +
            (AD9833_Control ->  square_wave << 5) +
            (AD9833_Control ->  DIV2 << 3) +
            (AD9833_Control ->  waveform << 1);

}

void AD9833_freq_load(uint8_t freqreg, uint32_t freq)
{

            uint16_t MSB = 0;
            uint16_t LSB = 0;
            uint32_t freq32 = 0;
            freq32 =  freq * pow(2,28)/MCLK;
            MSB = ((freq32 & 0xFFFC000) >> 14);
            LSB = (freq32 & 0x3FFF);
            if (freqreg == 0)
            {
                MSB &= ~(1 << 15);
                MSB |=  (1 << 14);
                LSB &= ~(1 << 15);
                LSB |=  (1 << 14);
            }
            if (freqreg != 0)
            {
                MSB |=  (1 << 15);
                MSB &= ~(1 << 14);
                LSB |=  (1 << 15);
                LSB &= ~(1 << 14);
                
           }

           AD9833_FREQ_SPI[0] = LSB;
           AD9833_FREQ_SPI[1] = MSB;
           //printf("LSB is %x and MSB is %x \n",LSB,MSB);
}

void AD9833_phase(uint8_t phasereg, uint16_t phase)

{
          uint16_t phase12 = 0;
          phase12 = (phase & 0xFFF);

          if (phasereg == 0)
          {
               
              phase12 |= (1 << 15) | ( 1 << 14); 
              phase12 &= ~(1 << 13);
              
          }
          if (phasereg != 0)
          {
              
              phase12 |= (1 << 15) | ( 1 << 14) | ( 1 << 13);
          }

AD9833_PHASE_SPI[0] = phase12;
//printf("phase reg is: 0x%x \n",AD9833_PHASE_SPI[0]);

}

uint16_t AD9833_phase_degrees(uint8_t phasereg, float deg)

{
       float baby = 360;
       
       if (deg > baby)
       {
        deg = round(fmod(deg,baby));
       }    

          uint16_t phase12 = 0;
    
          if (phasereg == 0)
          {
               
              phase12 |= (1 << 15) | ( 1 << 14); 
              phase12 &= ~(1 << 13);
              
          }
          if (phasereg != 0)
          {
              
              phase12 |= (1 << 15) | ( 1 << 14) | ( 1 << 13);
          }
    
    float fullrad = 6.283180;
    //float deg = 0; // end user wants this as phase offset
    const float pi = 3.14159; // Pi
    float rad = 0;
    float divsr = .00153397;   // value 2*pi/4096
   
    float phasebits = 0;
    uint16_t finalbits = 0;
    
    
    //convert to radians
    rad = deg * (pi/180);
    
    //the 12 bit number assumes radians * divsr so do that
     //printf ("radians is is = %f \n",rad);
    
    phasebits = rad/fullrad;
     //printf ("phasebits is  = %f \n",phasebits);
    
    finalbits = phasebits * 4096;

     //printf ("finalbits is  = %d \n",finalbits);
    
    
    return phase12 + finalbits;
}





void AD9833_Tri(void)
{
 
    AD9833_CNTL_SPI[0] |= (1<<1);
    AD9833_CNTL_SPI[0] &= ~(1<<5);
    AD9833_CNTL_SPI[0] &= ~(1<<8); //run
}

void AD9833_Sine(void)
{
 
   printf("In Sine function \n");
   AD9833_CNTL_SPI[0] &= ~(1<<1);
   AD9833_CNTL_SPI[0] &= ~(1<<5);
   AD9833_CNTL_SPI[0] &= ~(1<<8); //release reset; run
    
}

void AD9833_square(void)
{
 
    printf("In Square function \n");
    AD9833_CNTL_SPI[0] |= (1<<5);
    AD9833_CNTL_SPI[0] |= (1<<3);
    AD9833_CNTL_SPI[0] &= ~(1<<8); //run
}

void AD9833_square_halffreq(void)
{
    AD9833_CNTL_SPI[0] |= (1<<5);
    AD9833_CNTL_SPI[0] &= ~(1<<3);
    AD9833_CNTL_SPI[0] &= ~(1<<8); //run
 
}

void AD9833_reset(void)
{
       AD9833_CNTL_SPI[0] |= (1<<8);
}

void AD9833_powerup_reset(void)
        {
        AD9833_CNTL_SPI[0] = 0x100;
        }

void AD9833_release_reset(void)
{
    AD9833_CNTL_SPI[0] &= ~(1 << 8); // FLIP RESET BACK TO RUN
    

}

void AD9833_DAC_freeze(void)
{
    AD9833_CNTL_SPI[0] |= (1 << 7);
    AD9833_CNTL_SPI[0] &= ~(1 << 6);
    //printf("DAC freeze cntl settings: %x \n",AD9833_CNTL_SPI[0]);
}

void AD9833_DAC_unfreeze(void)
{
     AD9833_CNTL_SPI[0] &= ~(1 << 7);
     AD9833_CNTL_SPI[0] &= ~(1 << 6);
     //printf("DAC UNfreeze cntl settings: %x \n",AD9833_CNTL_SPI[0]);
}

void AD9833_freq1_to_output(void)
{
     AD9833_CNTL_SPI[0] |= (1 << 0xb); 
}

void AD9833_freq0_to_output(void)
{
     AD9833_CNTL_SPI[0] &= ~(1 << 0xb); 
}

void AD9833_phase1_to_output(void)
{
     AD9833_CNTL_SPI[0] |= (1 << 0xa); 
}

void AD9833_phase0_to_output(void)
{
     AD9833_CNTL_SPI[0] &= ~(1 << 0xa); 
}

void AD9833_Load_MSB_LSB(void) //  28 bit freq load
{
    AD9833_CNTL_SPI[0] |= (1 << 13);
}

void AD9833_Load_MSB_only(void)
{
    AD9833_CNTL_SPI[0] &= ~(1 << 13);
    AD9833_CNTL_SPI[0] |= (1 << 12);
}


void AD9833_Load_LSB_only(void)
{
    AD9833_CNTL_SPI[0] &= ~(1 << 13);
    AD9833_CNTL_SPI[0] &= ~(1 << 12);
}


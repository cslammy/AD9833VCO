#ifndef AD9833_H_INCLUDED
#define AD9833_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

#define CNTL_SEND SPI_TransferTx16_variable_num_words(spi0,AD9833_CNTL_SPI, 1)
#define FREQ_SEND SPI_TransferTx16_variable_num_words(spi0,AD9833_FREQ_SPI, 2)
#define PHASE_SEND SPI_TransferTx16_variable_num_words(spi0,AD9833_PHASE_SPI, 1)

//struct not used yet, but shows bit layout for AD9833
struct AD9833_C // struct for control data
{
   uint16_t CONTROL:2;     //bits 15-14     always 0b00 for control
   uint16_t B28:1;         //bit 13         1: write to reg write MSB then LSB (32 bits)   0: write to MSB **or** LSB (16 bits read, 16 bits ignored)
   uint16_t HLB:1;         //bit 12         1: freq reg write to MSB portion of freq reg only; ignore LSB data   0: freq write to LSB portion of freq reg; ignore MSB data **B28 must be 0**
   uint16_t FSELECT:1;     //bit 11         1: freq0 reg value to output   0: freq1 to reg value to output
   uint16_t PSELECT:1;     //bit 10         1: phase0 reg value to output   0: phase1 reg value to output
   uint16_t reserved0:1;   //bit 9          always value of 0b0
   uint16_t reset:1;       //bit 8          1: reset output to midscale  0: normal operation  Note, device needs reset upon power up
   uint16_t SLEEP:2;       //bit 7,6        0b00: normal operation  0b01: power down DAC  0b10: disable internal clock (freezes DAC) 0b11: clock and dac off
   uint16_t square_wave:1; //bit 5          1: square wave on  0: tri or sine on
   uint16_t reserved1: 1;  //bit 4          always value of 0b0
   uint16_t DIV2: 1;       //bit 3          1: sq wave freq normal    0: square wave freq half
   uint16_t reserved2:1;   //bit 2          always value of 0b0
   uint16_t waveform:1;    //bit 1          1: triangle out   0: sine out  **bit 5 must be 0**
   uint16_t reserved3:1;   //bit 0          always value of 0b0

};


//buffer global defs
static uint16_t AD9833_CNTL_SPI[1];  //global buffer for AD9833 control SPI writes
static uint16_t AD9833_FREQ_SPI[2];  //global buffer for AD9833 freq SPI writes
static uint16_t AD9833_PHASE_SPI[1]; //global buffer for AD9833 phase SPI writes
static uint16_t AD9833_init_data[6];  //initialize AD9833 buffer after resetstat

void AD9833_powerup_reset(void);

//initialize device sine wave, nominalfreq, phase=0
//phase 0 and freq 0 regs selected for output
//device = run
void AD9833_init(void);

//not used yet
void control_value_sum(void);

//load frequency registers
//freq reg 0 means use 0, anything else use 1
//freq frequency you want to see at output
void AD9833_freq_load(uint8_t freqreg, uint32_t freq);

//load phase registers with value 0-4095
//phasereg 0 means use 0, anything else use 1
//phase--phase you want to see at output
void AD9833_phase(uint8_t phasereg, uint16_t phase);

//load phase registers with offset value in degrees 
//phasereg 0 means use 0, anything else use 1
//deg  degrees offset
void AD9833_degrees(uint8_t phasereg, float deg);


//change to triangle wave at output
void AD9833_Tri(void);

//Change to sine wave at output
void AD9833_Sine(void);

//Change to square wave at output
void AD9833_square(void);

//change to squarewave at output
//square wave is half of freq in register
void AD9833_square_halffreq(void);

//reset the little mofo
void AD9833_reset(void);

//reset release; run device
void AD9833_release_reset(void);

//freeze current value in DAC to output
void AD9833_DAC_freeze(void);

//release DAC freeze
void AD9833_DAC_unfreeze(void);

//choose FREQ0 register for output
void AD9833_freq0_to_output(void);

//choose FREQ1 register for output
void AD9833_freq1_to_output(void);

//choose phase0 register for output
void AD9833_phase0_to_output(void);

//choose phase1 register for output
void AD9833_phase1_to_output(void);

//normal 28 bit freq loads
void AD9833_Load_MSB_LSB(void);

//MSB freq data to output only
void AD9833_Load_MSB_only(void);

//LSB freq data to output only
void AD9833_Load_LSB_only(void);


#endif // AD9833_H_INCLUDED

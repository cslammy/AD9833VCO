# AD9833VCO
 Volt-octave AD9833 audio VCO
 
 This is a work in progress.  

The ideas:
Different approach for a V/oct synthesizer VCO
We need pulse, saw, and optionally triangle at output
10V peak to peak straddling GND.
V per octave is sampled by a 12 bit DAC==currenty mcp3201 IC (may need to get replaced with 14 bit ADC?)
RP2040 dev board (as of 11-10-22, specifically a SEEED XAIO RP2040 dev board) hosts the RP2040 MCU

The dev board uses SPI to get an AD9833 FG IC to respond accurately to Volt per octave input control voltage.
 
I have developed and am still debutgging a carrier board for the SEEED dev board and the required buffers for accurate Volt/octave performance.

As of 11-10-22 the carrier board still has some errors, again, a work in progress
However, basic V-Oct creation of a triangle wave works from about 60hz to 20Khz.
See the work so far in action--for more information please see this blog post.
http://audiodiwhy.blogspot.com/2022/10/ad9833-rp2040-vco-voltoctave-is-working.html



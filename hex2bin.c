#include "hex2bin.h"

//this guy is the bomb--recurse me up Scotty!
//https://www.quora.com/How-can-you-convert-hexadecimal-to-binary-without-using-arrays-in-C-program
//check yer work
//https://www.rapidtables.com/convert/number/hex-to-binary.html


void hex2bin(int16_t h)

{
if (!h) return; //countdown if not zero run function again put h values on stack
hex2bin(h>>1);
printf("%d",h&1); //pop most recent value off stack and print it, so, low to high
return;

}

 

#include "UART.h"
#include <stdio.h>
 

void led_on(void)
{

       printf("turning on LED...\n");
       gpio_put(ONBOARD_LED,1);
      
}

void led_off(void)
{
 
       printf("turning off LED...\n");
       gpio_put(ONBOARD_LED,0);
     
}

uint32_t dumpleft_conv2dec(char *hi, uint8_t chopleft, uint8_t digits)
 {   
    uint8_t i, len = 0;
    uint32_t result = 0;
    char *x = hi;
    //printf("in dumpleft, string is: %s \n",x);
    uint8_t count = 0; 
   
    count = chopleft;
    do
    {
 
          {
        
        *(x + count - chopleft) = *(x + count);
         }
          count++;
    }
    while (*(x+count) != '\0');
   // printf("in dumpleft2, string is: %s \n",x);
    
    
    //deal with end of string 

   if (*(x + count) == '\0')
    {   
       // printf("slash zero found \n");
        *(x + count - chopleft) = '\0';
         
    }
    
   // printf("new string is: %s \n",x);
     
     
    //turn ASCII numbers into a decimel number
    
    for (i=0; i< digits; i++)
    {
      result = result * 10 + ( x[i] - '0' );
	}

	
    return result;  
 }



 uint8_t compare_strings(char *a, char *b, uint8_t len)
{
   uint8_t x = 0;

   if ((strncmp(a,b,len))==0)
   {
     x = 1;
     return x;
   }   
   else
   {
       x = 0;
       return x;
   }
    
}

   
#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include "pico/bootrom.h"
#include "string.h"
#include "hardware/gpio.h"


//good vid for this https://www.youtube.com/watch?v=pbWhoJdYA1s
 
// global buffer for uart reads
static char uart_buffer[1024]; 

 //******************MACROS ***************************

//put this in endless loop in main
#define READ_UART_BUFFER scanf("%1024s",uart_buffer) 

#define ONBOARD_LED 25

//test to see if string X is in buffer. 
//Do whatever is in curlies {...} if there is a match
#define UART_READ(X) strcmp(uart_buffer, X)== 0

//enable Pico on board LED
#define PICO_ONBOARD_LED_INIT  gpio_init(ONBOARD_LED);gpio_set_dir(ONBOARD_LED,GPIO_OUT);

 //******************FUNCTIONS  ***************************

//LED_ON test--lites pico on board LED--UART not needed
void led_on(void);

//LED_OFF test--lites pico on board LED--UART not needed
void led_off(void);

//*********************************************
//*hi--string array with letter(s) at left and then only numbers, eg: F1234
//chopleft -- number of letters on the left you want the algorithm to remove
//digits--number of digits from left in the resulting string you want to convert to decimel.
uint32_t dumpleft_conv2dec(char *hi, uint8_t chopleft, uint8_t digits);



//*********************************************

//see if the leftmost len characters match.
//if a match return 1-true
//otherwise return 0-false
uint8_t compare_strings(char *a, char *b, uint8_t len);
 
#endif // UART_H_INCLUDED
/**********************************************************
* debug_printf.c for lab 6

 *   Author: Shengwei Peng - pengs
 *   Date Created: --- 
 *   Last Modified by: Shengwei Peng
 *   Date Last Modified: Oct 10 2016
 *   Assignment: lab6
 *   Partner:  Brandon Dennett - bdennett 
 */ 
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

// Delay function added for button push
// Stops the program for a second so user has chance to stop pressing button
void delay(void) {
  int i = 4000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void){

  // Initialize board
  f3d_led_init();
  f3d_uart_init();
  f3d_user_btn_init();
  int i, c, d = 0;

  
  while(1){
    i = i % 2;       // Counter used for LEDs
    c = c % 2;       // Boolean for button push
    
    // Turn LEDs on
    if (i == 0) {
      f3d_led_on(9);
      d++;          // Counter for how many times LED is turned on

      // Checks if user pushed the button and toggled printf statements on
      // Only prints when LED turns on
      if (c == 1) {
	printf("LED on.\n");
	printf("Current counter for led flashes: %d\n", d);
      }
      
    } else {
      f3d_led_off(9);  // Turns the LED off
      if (c == 1) printf("LED off.\n");
    }
    // Checks if the user presses the button and toggles printf on/off
    if (user_btn_read() == 1) {
      c++;
      printf("Someone pushed the button.\n");
      if (c == 1) printf("Printf debugger turned on.\n");
    }

    i++;
    delay();
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}

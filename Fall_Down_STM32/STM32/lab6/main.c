

/********************************************************** 
 * 
 *    Main.c
 * 
 * 
 *   Author: Shengwei Peng - pengs
 *   Date Created: --- 
 *   Last Modified by: Shengwei Peng
 *   Date Last Modified: Oct 10 2016
 *   Assignment: lab6
 *   Partner:  Brandon Dennett - bdennett 
 */ 


#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMER 20000
/*
void delay(int count) {
  while (count-- > 0) {
    int i = 10000; 
    while (i-- > 0) {
      asm("nop");/* This stops it optimising code out 
    }
  }
} */

int main(void) {
  f3d_lcd_init();
  f3d_uart_init();
  f3d_gyro_init();

int i, c, xC, yC, zC = 0;
  float f[3];
  char x[16];
  char y[16];
  char z[16];
  int xInt, yInt, zInt;
  
  f3d_lcd_fillScreen(BLACK);
  while(1){

    f3d_gyro_getdata(f);
    sprintf(x, "x: %f", f[0]);
    sprintf(y, "y: %f", f[1]);
    sprintf(z, "z: %f", f[2]);

    xInt = f[0];
    yInt = f[1];
    zInt = f[2];

    printf("x: %f\n", f[0]);
    //printf("y: %f\n", f[1]);
    //printf("z: %f\n", f[2]);

    f3d_lcd_drawChar(5, 80, 'X', GREEN, BLACK);
    f3d_lcd_drawChar(5, 100, 'Y', WHITE, BLACK);
    f3d_lcd_drawChar(5, 120, 'Z', BLUE, BLACK);
    //f3d_lcd_fillScreen(RED);
    f3d_lcd_drawString(0, 0, x, GREEN, BLACK);
    f3d_lcd_drawString(0, 10, y, WHITE, BLACK);
    f3d_lcd_drawString(0, 20, z, BLUE, BLACK);
    //int i = 0;
    
    if (xInt < 0) xInt = xInt*(-1);
    if (yInt < 0) yInt = yInt*(-1);
    if (zInt < 0) zInt = zInt*(-1);

    for (i = 10; i < xInt/3; i++) {
       for (c = 90; c < 94; c++) f3d_lcd_drawPixel(i, c, GREEN);}
    for (i = xInt/3; i < 129; i++) {
       for (c = 90; c < 94; c++) f3d_lcd_drawPixel(i, c, BLACK);}

    for (i = 10; i < yInt/3; i++) {
       for (c = 110; c < 114; c++) f3d_lcd_drawPixel(i, c, WHITE);}
    for (i = yInt/3; i < 129; i++) {
       for (c = 110; c < 114; c++) f3d_lcd_drawPixel(i, c, BLACK);}

    for (i = 10; i < zInt/3; i++) {
       for (c = 130; c < 134; c++) f3d_lcd_drawPixel(i, c, BLUE);}
    for (i = zInt/3; i < 129; i++) {
       for (c = 130; c < 134; c++) f3d_lcd_drawPixel(i, c, BLACK);}

    delay(10);
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

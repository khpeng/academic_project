
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
 *   Partner:  Shengyu Chen
 */ 


#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>

#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define TIMER 20000

int main(void) {
// initialization 
  f3d_uart_init();
  delay(10);
  f3d_lcd_init();
  f3d_lcd_fillScreen(WHITE);
 
  delay(10);
  f3d_i2c1_init();
  delay(10); 
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10); 


// Setup buffers
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

// floats arrays
float coor[3];
char data[10];

// strings for printing
unsigned char x[50];
unsigned char y[50];
unsigned char string_roll[50];
unsigned char string_pitch[50];
unsigned char string_heading[50];
//unsigned char string_heading_degrees[50];

float roll;
float pitch;
float heading;
float degrees;   
int led;
//draw string

  

while(1) {

f3d_accel_read(coor); 
//use given formula to calculate pitch, roll and heading.
    pitch = ((atanf((coor[0]) / (sqrtf((powf(coor[1], 2.0)) + (powf(coor[2], 2.0)))))));
    roll = ((atanf((coor[1]) / (sqrtf((powf(coor[0], 2.0)) + (powf(coor[2], 2.0)))))));
    f3d_mag_read(coor);
    float xH =  coor[0]* cosf(pitch) + coor[2] * sinf(pitch);
    float yH = ( coor[0] * sinf(roll) * sinf(pitch)) + (coor[1] * cosf(roll))- (coor[2] * sinf(roll) *cosf(pitch));
    heading =((atan2f(yH,xH)  * 180) / M_PI);
    
// turn float into string
    sprintf( string_pitch, "Pitch = %f", pitch);
    sprintf( string_roll, "Roll = %f",   roll); 
//use drawstring to draw string on lcd
    f3d_lcd_drawString(0, 5, string_pitch, BLACK, WHITE);
    f3d_lcd_drawString(0, 35, string_roll, BLACK, WHITE);

//  x, y tilt angle
   float y1 = (pitch * (180/M_PI));
   float x1 = (roll * (180/M_PI));
//turn float into string
   sprintf(y , "Y-angle = %f", (pitch * (180/M_PI))); 
   f3d_lcd_drawString(0, 15, x, BLACK, WHITE);
   sprintf(x , "X-angle = %f", (roll * (180/M_PI))); 
   f3d_lcd_drawString(0, 25, y, BLACK, WHITE);
//draw tilt angle as string on lcd
   f3d_lcd_drawChar(0, 45, 'X', BLACK, WHITE);
   f3d_lcd_drawChar(0, 55, 'Y', BLACK, WHITE);

//Step: 8  use for loop and draw pixel to draw bar

  int i,c;
   if (x1 < 0) x1 = x1*(-1); // Turn negative value into positive value
   if (y1 < 0) y1 = y1*(-1);
  

   for (i = 20; i < x1*2; i++) {
           for (c = 50; c < 55; c++) f3d_lcd_drawPixel(i, c, BLACK);}
   for (i = 20; i < y1*2; i++) {
           for (c = 60; c <65; c++) f3d_lcd_drawPixel(i, c, BLACK);}
   // delay(100);
   for (i = 20; i < 129; i++) {
          for (c = 50; c < 55; c++) f3d_lcd_drawPixel(i, c, GREEN);}
   for (i = 20; i < 129; i++) {
          for (c = 60; c < 65; c++) f3d_lcd_drawPixel(i, c, GREEN);}


   //draw heading on lcd
   f3d_mag_read(coor);
   sprintf(data,"%.3f", coor[0]);
   sprintf(string_heading, "Heading = %f", heading);
   f3d_lcd_drawString(0, 75, string_heading, BLACK, WHITE);
    
  
//Step: 8  we use character(N, S, E, W) to show direction and draw the string on lcd 

if(heading >= -45  && heading <= 45  ){
  f3d_lcd_drawString(60, 100, "South", BLACK, WHITE);
 } else if (heading > 45  && heading < 135 ){
  f3d_lcd_drawString(60, 100, "East", BLACK, WHITE);
 } else  if( (heading <= -135  && heading >= -180 ) || (heading >= 136 && heading <= 180)) {
  f3d_lcd_drawString(60, 100, "West", BLACK, WHITE);
 }  else {
  f3d_lcd_drawString(60, 100, "North", BLACK, WHITE);
 }
}
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
while (1);
}
#endif


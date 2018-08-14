
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
 */
#define TIMER 20000
#define BG BLUE        // Background color constant

// Draw bars for pitch/roll values
// Draw to right for positive value, left for negative value
// Also draws '+' and '-' chars based on color
// Bars start in center of screen, user designates y value
void drawbarSign(uint16_t color, int value, int y0) {

  int x, y;

  value = value * .7;      // constrains bar to screen dimensions

  f3d_lcd_drawChar(96,(y0 + 10),'+',color,BG);
  f3d_lcd_drawChar(32,(y0 + 10),'-',color,BG);

  // Positive values = draw bar to right
  if (value > 0) {
    for (x = 64; x < (value + 64); x++){
      for (y = y0; y < (y0 + 4); y++) f3d_lcd_drawPixel(x, y, color);}
    for (x = (value + 64); x < 129; x++){
      for (y = y0; y < (y0 + 4); y++) f3d_lcd_drawPixel(x, y, BG);}
    // Wipe remnants of negative bar
    for (x = 0; x < 64; x++){
      for (y = y0; y < (y0 + 4); y++) f3d_lcd_drawPixel(x, y, BG);}

  // Negative values = draw bar to left
  } else {
    for (x = 64; x > (value + 64); x--){
      for (y = y0; y < (y0 + 4); y++) f3d_lcd_drawPixel(x, y, color);}
    for (x = (64 + value); x > 0; x--){
      for (y = y0; y < (y0 + 4); y++) f3d_lcd_drawPixel(x, y, BG);}
    // Wipe remnants of positive bar
    for (x = 128; x > 65; x--){
      for (y = y0; y < (y0 + 4); y++) f3d_lcd_drawPixel(x, y, BG);}
  }
}

// Draws bars from left to right of screen
// Used for nunchuk values
void drawBar(int x, int y, int val, uint16_t color){

  int i, c; // counters

  for (i = x; i < val; i++) {
    for (c = y; c < y + 4; c++) f3d_lcd_drawPixel(i, c, color);}
  for (i = val; i < 129; i++) {
    for (c = y; c < y + 4; c++) f3d_lcd_drawPixel(i, c, BG);}

}

// Converts heading data into compass
// Lights up LEDs based on which direction it's reading
void compass(float head){

  f3d_led_all_off();

  if ((head <= 180 && head >= 135) || (head >= -180 && head < -135)){
    f3d_led_on(9);    // North
    f3d_lcd_drawString(49, 30, "North", YELLOW, BG);
  } else if (head >= -135 && head < -45) {
    f3d_led_on(11);   // East
    f3d_lcd_drawString(49, 30, "East ", GREEN, BG);
  } else if (head >= -45 && head < 45) {
    f3d_led_on(13);   // South
    f3d_lcd_drawString(49, 30, "South", MAGENTA, BG);
  } else if (head >= 45 && head < 135) {
    f3d_led_on(15);     // West
    f3d_lcd_drawString(49, 30, "West ", RED, BG);
  }

}
  

int main(void) {
   setvbuf(stdin, NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0);

   // Set up your inits before you go ahead
   f3d_i2c1_init();
   delay(10);
   f3d_accel_init();
   delay(10);
   f3d_mag_init();
   delay(10);
   f3d_nunchuk_init();
   delay(10);

   f3d_lcd_init();
   f3d_user_btn_init();
   f3d_uart_init();
   f3d_led_init();

   // Variables
   int i, c;               // counter variables
   int button;             // button counter
   nunchuk_t nun;          // nunchuk struct
   float a[3], m[3];       // arrays for accel/mag
   float pitch, roll, temp, xH, yH, heading;      // floats
   char p[16], r[16], h[16]; // char arrays for printing pitch/roll/heading

   // Initial wipe the screen and turn off all LEDs
   f3d_lcd_fillScreen(BG);
   f3d_led_all_off();

   while(1){
     
     // Read nunchuk data
     f3d_nunchuk_read(&nun);

     // Constrain button boolean to value of 0 or 1
     button = button % 3;

     // Pitch/Roll screen
     if (button == 0){

       // Read accel data
       f3d_accel_read(a);
       
       // Calculate pitch
       temp = (a[1] * a[1]) + (a[2] * a[2]);
       pitch = atan(a[0]/sqrt(temp));

       // Calculate roll
       temp = (a[2] * a[2]) + (a[0] * a[0]);
       roll = atan(a[1]/sqrt(temp));

       // Convert values from radians to degrees
       pitch = pitch * (180/M_PI);
       roll = roll * (180/M_PI);

       // Convert floats to strings
       sprintf(p, "Pitch: %f", pitch);
       sprintf(r, "Roll: %f", roll);

       // Draw pitch label/value and bar
       f3d_lcd_drawString(15,25,p,GREEN,BG);
       drawbarSign(GREEN,pitch,35); 

       // Draw roll label/value and bar
       f3d_lcd_drawString(15,65,r,WHITE,BG);
       drawbarSign(WHITE,roll,75);
     } 

     // Heading screen
     if (button == 1){

       // Read accel & mag data
       f3d_accel_read(a);
       f3d_mag_read(m);

       // Calculate pitch
       temp = (a[1] * a[1]) + (a[2] * a[2]);
       pitch = atan(a[0]/sqrt(temp));

       // Calculate roll
       temp = (a[2] * a[2]) + (a[0] * a[0]);
       roll = atan(a[1]/sqrt(temp));

       // Convert values from radians to degrees
       pitch = pitch * (180/M_PI);
       roll = roll * (180/M_PI);

       // Compute heading and convert to string
       xH = (m[0] * cos(pitch)) + (m[2] * sin(pitch));
       yH = (m[0] * sin(roll) * sin(pitch)) + (m[1] * cos(roll)) - 
	 (m[3] * sin(roll) * cos(pitch));
       heading = (atan2(yH, xH) * 180)/M_PI;

       // Convert heading to string
       sprintf(h, "%f", heading);

       // Draw label & value for heading on LCD screen
       f3d_lcd_drawString(40, 10, "Heading:", WHITE, BG);
       f3d_lcd_drawString(31, 20, h, WHITE, BG);

       // Light up LEDs based on heading value
       // Also writes heading direction as word on LCD
       compass(heading);

     }

     if (button == 2) {
       
       // Draw screen labels
       f3d_lcd_drawString(43, 2, "Nunchuk", WHITE, BG);
       f3d_lcd_drawChar(10, 15, 'X', GREEN, BG);
       f3d_lcd_drawChar(10, 40, 'Y', WHITE, BG);
       f3d_lcd_drawChar(10, 65, 'Z', YELLOW, BG);
       f3d_lcd_drawString(10, 90, "Joystick Up/Down:", MAGENTA, BG);
       f3d_lcd_drawString(10, 115, "Joystick Left/Right", RED, BG);
       
       // Draw nunchuk bars
       drawBar(10, 25, nun.ax * .12, GREEN);
       drawBar(10, 50, nun.ay * .12, WHITE);
       drawBar(10, 75, nun.ax * .12, YELLOW);
       drawBar(10, 100, nun.jy * .46, MAGENTA);
       // Hitting values of 255 or 0 will move to the next/prev screen
       drawBar(10, 125, nun.jx * .46, RED);

     }

     // Checks when user pushes button
     // If so, clear the screen and reset LEDs
     // Z = next screen; C = prev screen
     // Full right on joystick = next screen
     // Full left on joystick = prev screen
     // Everytime the screen changes, wipe it and turn off LEDs
     if ((nun.z == 1) || (nun.jx == 255)) {
       button++;
       f3d_lcd_fillScreen(BG);
       f3d_led_all_off();
       delay(375);
     }
     if ((nun.c == 1) || (nun.jx == 0)) {
       button--;
       f3d_lcd_fillScreen(BG);
       f3d_led_all_off();
       if (button < 0) button += 3;       // Check for negative button value
       delay(375);
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

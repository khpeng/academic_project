//Shengwei Peng, Yifan Liu, Shengyu Chen.
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers
#include <f3d_user_btn.h>
#include <f3d_led.h>
#include <f3d_systick.h>
#include <queue.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <stdio.h>
#include <math.h>




int main(void){
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_delay_init();
  delay(10);
  f3d_user_btn_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_led_init();
  delay(10);
  f3d_systick_init();
  delay(10);
  
  f3d_led_all_off();

  SysTick_Handler();
  while(1){
    
    putchar(getchar());
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

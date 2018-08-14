
/*
Shengwei Peng (pengs)
 */
// Simple looping delay function
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {

  // make light open
   f3d_led_init();
   
   f3d_user_btn_init();
   //GPIOE->BSRR = GPIO_Pin_9;
   int index= 9;
   while(1){
     if(user_btn_read() != 1){
       f3d_led_all_off();
     if (index == 16){
       f3d_led_on(8);
       delay();
       f3d_led_all_on();
       delay();
       f3d_led_all_off();
       index = 9;
     }
     f3d_led_on(index);
     delay();
     f3d_led_off(index);
     ++index;
     }else{
       f3d_led_on(index);  
     }
     
     /*
     if(user_btn_read()){
       f3d_led_all_on();
     }else f3d_led_all_off();
     */
     }
   // MAKE LIGHT CLOSE 
  // GPIOE->BRR = GPIO_Pin_9;
   }

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

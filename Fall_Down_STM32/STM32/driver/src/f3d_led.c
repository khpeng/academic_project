//Last Modified By:	James Gregory
//			gregojam
//
//Partner:		Ke Zhao
//			zhaok


/************************
 *f3d_led.c - contains intializations/functions for the leds
 ************************/

#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <f3d_led.h>


//intializes the port and pins for the leds on the board
void f3d_led_init(void) {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*Turns on the appropriate led as specified by the parameter.*/
void f3d_led_on(int led) {
	GPIOE->BSRR = GPIO_Pin_8 << led;
}

/*Turns off the approiate led as specified by the parameter*/ 
void f3d_led_off(int led) {
	GPIOE->BRR = GPIO_Pin_8 << led;
} 

/*Turns on all LEDs*/
void f3d_led_all_on(void) {
	GPIOE->BSRR = GPIO_Pin_All;
} 

/*Turns off all LEDs*/
void f3d_led_all_off(void) {
  GPIOE->BRR = GPIO_Pin_All;
} 

/* led.c ends here */


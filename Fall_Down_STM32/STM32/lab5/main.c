//main.c for lab5

//main.c for lab6
/*
UserName : pengs
Name: Shengwei Peng
Partner : zhang435
Name : Jiawei Zhang
 */
 
#include <stm32f30x.h>
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

void delay(void) {

  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void){
  // init light , print grav sensor , and user button
  f3d_led_init();
  f3d_uart_init();
  f3d_gyro_init();
  f3d_user_btn_init();

  // allow us to use print statement
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  // var to store the X-Y-Z grav data 
  float a[3] = {0};
  // use to count the current testing (X Y Z)
  int curr = 0;
  // use to store the user input
  int c;

  while(1){
    // If I get the user input
    // store the input , check which one user what to check 
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != (uint16_t)RESET){
      c = USART_ReceiveData(USART1);
      if (c == 'x'){
	printf("Change Mode to : X\n");
	curr = 0;
      }

      else if (c == 'y'){
	printf("Change Mode to : Y\n");
	curr =1;
      }

      else if (c == 'z'){
	printf("Change Mode to : Z\n");
	curr =2;
      }else{
	printf("The char is not in the record");
      }
    }

    // if user press the button ++ the current testing deminsion 
    if(user_btn_read()){
      if (curr ==0){
	printf("change Mode to : Y\n");
	++curr;
      }
      else if (curr ==1){
	printf("Change Mode to : Z\n");
	++curr;
      }else if (curr == 2){
	// if it is the mode 3 ,next mode will start from mode 1
	printf("Change Mode to : X\n");
	curr =0;
      }
    }
    printf("current %i \n",curr);
    // the allow us to get the data from gav sensor 
    f3d_gyro_getdata(a);
    
    if (a[curr]>=150){
      f3d_led_on(13);}

    if (a[curr]>=100){
      f3d_led_on(14);}
    
    if (a[curr]>=70){
      f3d_led_on(15);}
    
    if (a[curr]>30){
      f3d_led_on(8);}
    
    if (a[curr] > 5){
      f3d_led_on(9);}
     
    if (a[curr] < -150){
      f3d_led_on(13);}
    
    if (a[curr] < -100){
      f3d_led_on(12);}
    
    if (a[curr] <-70){
      f3d_led_on(11);}
     
    if (a[curr] < -30){
      
       f3d_led_on(10);}
     
    if (a[curr] < -5){
      f3d_led_on(9);}
    // CALL THE DELAY , since the light goes to fat , without delay , we can not see anything
    delay();
    // after the light on , we also need to turn off the light
    f3d_led_all_off();
    // print all the data 
    printf("%f %f %f\n",a[0],a[1],a[2]);
  } 
  return 0 ;
}

void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}

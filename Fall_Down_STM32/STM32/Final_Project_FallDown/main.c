#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
//#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;
FIL fid;
BYTE Buff[512];
int ret;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

float absFloat(float value) {
  if(value < 0) return(value*-1);
  else return value;
}




//function that used when ball is on the board.
void stay_on_bar(int *x_axis, int *y_axis, int *speed_of_ball, int *bar_x, int *bar_y, int* bool, uint16_t *color, nunchuk_t nun){
  if((*x_axis>*bar_x-5 && *x_axis<*bar_x+40) && (*bar_y-*y_axis>=0 && *bar_y-*y_axis<=10)){
   
    f3d_lcd_drawString(*bar_x-5, *bar_y, "o", WHITE, WHITE);
    f3d_lcd_drawString(*bar_x+40, *bar_y, "o", WHITE, WHITE);

    if (*color == (uint16_t) RED){
      if (nun.jx==0xff){//Read data from nunchuck. control the ball moving left or right.
	*x_axis = *x_axis - *speed_of_ball;
      }
      else if (nun.jx==0x00) {
	*x_axis = *x_axis + *speed_of_ball;
      }   
    }
    if (*color == (uint16_t) BLACK){
      if (nun.jx==0xff){//Read data from nunchuck. control the ball moving left or right.
	*x_axis = *x_axis + *speed_of_ball;
      }
      else if (nun.jx==0x00) {
	*x_axis = *x_axis - *speed_of_ball;
      }   
    }
    *y_axis=*bar_y-5;
    *bool = 1;
  } 
}




void pass_the_bar(int *ball_y_axis, int *bar_y, int *score, int* pass_bool){
  if ((*ball_y_axis - *bar_y >= 0) && (*pass_bool == 1)) {
    *score = *score + 1;
    *pass_bool = 0;
  }
}

int playMusic(char filename[]){
  FRESULT rc;		       
  f_mount(0, &Fatfs);
  printf("\nOpen file\n");
  printf("\nOpen %s\n", filename);
  rc = f_open(&fid, filename, FA_READ);

  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    // readckhd
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
		 
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    //audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
 
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }

    audioplayerStop();

  }  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  if (rc) die(rc);
}

int game(void) {

  f3d_led_all_on();

  f3d_lcd_fillScreen2(WHITE);

  int board_height = 155;
  int board_width = 88;

  f3d_lcd_bar(0, 0, 127, 8, BLACK);

  nunchuk_t nun;  // declare nunchuk
  f3d_nunchuk_read(&nun);

  int btn = 0;
  //variable neede for moving bars.
  int index_speed = 0;
  float speed[] = {1, 2, 3, 4}; 
  int bar1 = 40;
  int bar2 = 80;
  int bar3 = 120;
  int bar4 = 155;
  int random1 = rand() % 88;
  int random2 = rand() % 88;
  int random3 = rand() % 88;
  int random4 = rand() % 88;
  /* int random1 = abs(nun.ax % 100 - 12);
  int random2 = abs(nun.ax % 100 - 12);
  int random3 = abs(nun.ax % 100 - 12);
  int random4 = abs(nun.ax % 100 - 12);*/
  int index_color1 = 0;
  int index_color2 = 0;
  int index_color3 = 0;
  int index_color4 = 0;
  int number_of_floor = 97;
  uint16_t color_of_bar[] = {BLACK, BLACK, BLACK, BLACK, BLACK, RED};
  //variable needed for dot.
  int x_axis = rand() % 123;
  int y_axis = 8;
  int speed_of_ball = 4;
  int falling_speed_of_ball = 2;
  int bool;//determines whether ball is stayed on the board.

  //varible used to determine wether the ball passed the bar.
  int pass_bool1 = 1;
  int pass_bool2 = 1;
  int pass_bool3 = 1;
  int pass_bool4 = 1;
  //------------------
  int z;
  int score = 0;
  char str0[80];
  char str00[80];
  //variable to play music
  FRESULT rc;
  DIR dir;
  FILINFO fno;
  UINT bw, br;
  unsigned int retval;
  int bytesread;

  while (1){
    //if the button is pushed, the game will paused.
    if (user_btn_read()) {
      btn = btn + 1;
      delay(100);
    } 
    if (btn % 2 == 1) {
      f3d_lcd_drawString(0,0 , "Pause", WHITE, BLACK);
      while(1){
	if(user_btn_read()) {
	  delay(100);
	  btn = btn + 1;
	  f3d_lcd_bar(0, 0, 127, 8, BLACK);
	  break;
	} 
      }
    }

    f3d_nunchuk_read(&nun);

    //implementation of the moving bars.
    //first bar
    f3d_lcd_bar(random1, bar1+speed[index_speed], random1+40, bar1+speed[index_speed]+5, WHITE);
    f3d_lcd_bar(random1, bar1+1, random1+40, bar1+5, color_of_bar[index_color1]);
    //second bar
    f3d_lcd_bar(random2, bar2+speed[index_speed], random2+40, bar2+speed[index_speed]+5, WHITE);
    f3d_lcd_bar(random2, bar2+1, random2+40, bar2+5, color_of_bar[index_color2]);
    //third bar
    f3d_lcd_bar(random3, bar3+speed[index_speed], random3+40, bar3+speed[index_speed]+5, WHITE);
    f3d_lcd_bar(random3, bar3+1, random3+40, bar3+5, color_of_bar[index_color3]);
    //fourth bar
    f3d_lcd_bar(random4, bar4+speed[index_speed], random4+40, bar4+speed[index_speed]+5, WHITE);
    f3d_lcd_bar(random4, bar4+1, random4+40, bar4+5, color_of_bar[index_color4]);

    bar1 = bar1 - speed[index_speed];
    if (bar1<=2) {
  //reverse that will reverse the direct of the ball, if it is 0, do NOT reverse, otherwise, reverse.
      index_color1 = rand() % 6;
      bar1=rand() % 5 + 160;
      random1 = rand() % 88;
      //int random2 = abs(nun.ax % 100 - 12);
      number_of_floor = number_of_floor - 1;
      pass_bool1 = 1;
    }

    bar2=bar2-speed[index_speed];
    if (bar2<=2) {
   //reverse that will reverse the direct of the ball, if it is 0, do NOT reverse, otherwise, reverse.
      index_color2 = rand() % 6;
      bar2=rand() % 5 + 175;
      random2 =  rand() % 88;
      //int random2 = abs(nun.ax % 100 - 12);
      number_of_floor = number_of_floor - 1;
      pass_bool2 = 1; 
    }
    bar3=bar3-speed[index_speed];
    if (bar3<=2) {
   //reverse that will reverse the direct of the ball, if it is 0, do NOT reverse, otherwise, reverse.
      index_color3 = rand() % 6;
      bar3=rand() % 5 + 190; 
      random3 =  rand() % 88;
      //int random2 = abs(nun.ax % 100 - 12);
      number_of_floor = number_of_floor - 1;
      pass_bool3 = 1;
    }

    bar4=bar4-speed[index_speed];
    if (bar4<=2) {
   //reverse that will reverse the direct of the ball, if it is 0, do NOT reverse, otherwise, reverse.
      index_color4 = rand() % 6;
      bar4=rand() % 5 + 205; 
      random4 =  rand() % 88;
      //int random2 = abs(nun.ax % 100 - 12);
      number_of_floor = number_of_floor - 1; 
      pass_bool4 = 1;
    }
    
    
    if (number_of_floor > 80){
      index_speed = 1;
    } 
    else if (number_of_floor > 60) {
      index_speed = 2;
    }
    else {
      index_speed = 3;
    }
    //end of implementation of the moving bars.
    //    delay(20);
    //implementation of the dot.
    
    printf("%d %d %d \n", nun.ax, nun.ay, nun.az);
    bool = 0;

    //if bool = 0, means ball is stay on the bar. then run to function. if one of them run. bool will be set to 1. Thus, only one will be run.
    stay_on_bar(&x_axis, &y_axis, &speed_of_ball, &random1, &bar1, &bool, &color_of_bar[index_color1],nun);
    stay_on_bar(&x_axis, &y_axis, &speed_of_ball, &random2, &bar2, &bool, &color_of_bar[index_color2],nun);
    stay_on_bar(&x_axis, &y_axis, &speed_of_ball, &random3, &bar3, &bool, &color_of_bar[index_color3],nun);
    stay_on_bar(&x_axis, &y_axis, &speed_of_ball, &random4, &bar4, &bool, &color_of_bar[index_color4],nun);

    //if bool = 0, means the ball will keep falling. 
    if (bool == 0) {//speed of tha ball falling.
      y_axis = y_axis + falling_speed_of_ball;
      pass_the_bar(&y_axis, &bar1, &score, &pass_bool1);
      pass_the_bar(&y_axis, &bar2, &score, &pass_bool2);
      pass_the_bar(&y_axis, &bar3, &score, &pass_bool3);
      pass_the_bar(&y_axis, &bar4, &score, &pass_bool4);

      if (nun.jx==0xff){//Read data from nunchuck. control the ball moving left or right.
	x_axis = x_axis + speed_of_ball;
	if(x_axis > 133 ) { 
	  x_axis = -6;
	}
      }
      else if (nun.jx==0x00) {
	x_axis = x_axis - speed_of_ball;
	if (x_axis < -8) {
	  x_axis = 128;
	}         
      }
    }
    //refresh the falling trail of dot.
    f3d_lcd_drawString(x_axis-speed_of_ball, y_axis-falling_speed_of_ball, "o", WHITE, WHITE);
    f3d_lcd_drawString(x_axis+speed_of_ball, y_axis-falling_speed_of_ball, "o", WHITE, WHITE);
    f3d_lcd_drawString(x_axis, y_axis-falling_speed_of_ball, "o", WHITE, WHITE);
    //stay_on_bar function will refresh the rising trail of dot as WHITE. 
    
    sprintf( str0, "%i", score);// SCORE  display
    f3d_lcd_drawString(100, 0, str0, WHITE, BLACK);

    f3d_lcd_drawString_DOT(x_axis, y_axis, "o", BLACK, WHITE);

    //implementation of end_game.
    if ((y_axis > 160) || (y_axis <= 9) || score == 10){
      break;
    }
  }
  f3d_lcd_fillScreen2(YELLOW);

  if(score != 10){

    sprintf( str0, "Score: %i", score);
    f3d_lcd_drawString(36, 80, "game over", BLACK, YELLOW);
    f3d_lcd_drawString(36, 60, str0, BLACK, YELLOW);
    f3d_led_all_off();
    playMusic("Lose.wav");

  } else {

    sprintf( str00, "Score: %i", score);
    f3d_lcd_drawString(40, 80, "You Win!!", BLACK, YELLOW);   
    f3d_lcd_drawString(36, 60, str00, BLACK, YELLOW);
    f3d_led_all_off();
    playMusic("Lose.wav");
     
 
  }  

  
  while (1){
    f3d_nunchuk_read(&nun);
    f3d_lcd_drawString(15, 100, "Press C to replay", BLACK, YELLOW);
    if (nun.c == 0x01){
      game();
    }
  }
  return 0;
}


void menu() {
  f3d_lcd_fillScreen2(BLACK);
  f3d_lcd_drawString(50, 20, "Rules", WHITE, BLACK);
  f3d_lcd_drawString(10, 40, "Use nunchuk to control ball to move",  WHITE, BLACK);
  f3d_lcd_drawString(10, 60, "Red bar will reverse direction of the ball",  WHITE, BLACK);
  f3d_lcd_drawString(10, 80, "Get on the 100th bar to win", WHITE, BLACK);
  f3d_lcd_drawString(10, 100, "Press user button to pause." , WHITE, BLACK);
  f3d_lcd_drawString(10, 120, "Press C to start game.", WHITE, BLACK);
}

void init(void){
  f3d_led_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_timer2_init();
  delay(10);
  f3d_dac_init();
  delay(10);
  f3d_delay_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_systick_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_nunchuk_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
}

int main(void) {
  init();
  menu();

  while(1){ 
    nunchuk_t nun;
    f3d_nunchuk_read(&nun);
    if( nun.c == 0x01) {
      game();
    }
  }
  return 0; 
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
  }
#endif

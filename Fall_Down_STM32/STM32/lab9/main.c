
//Lab 9
/* main.c 
 * Filename: main.c
 * Jeremy Benedek and Shengwie Peng 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: Nov 12
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include <math.h>

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

/////Vars
FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE Buff[128];		/* File read buffer */
float accel_data[3];
float pitch;
float roll;
uint16_t direction;

//nunchuck and orientation vars
int orientation = 0;
int selection = 0;
int old_z = 0;
int old_c = 0;
int curr_z = 0;
int curr_c = 0;

//draw image variables
FRESULT rc;
uint16_t color;
UINT bw, br;
uint16_t clr[128];
nunchuk_t nundata;

//choose image in the folder
char *image[] = {"0.bmp","1.bmp","2.bmp"};



/////STRUCTS
typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} bmp;
bmp Imgbuf[128];
typedef struct {
  unsigned char magic [2];
} bmpfile_magic;
typedef struct {
  uint32_t filesz ;
  uint16_t creator1 ;
  uint16_t creator2 ;
  uint32_t bmp_offset ;
} bmpfile_header;
typedef struct {
  uint32_t header_sz ;
  int32_t width ;
  int32_t height ;
  uint16_t nplanes ;
  uint16_t bitspp ;
  uint32_t compress_type ;
  uint32_t bmp_bytesz ;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors ;
  uint32_t nimpcolors ;
} BITMAPINFOHEADER ;

bmpfile_magic magic;
bmpfile_header header;
BITMAPINFOHEADER info;
int button;

//////METHODS
void inits(void);
void filestuff(void);
uint8_t get_direction();
void draw(char filename[], uint8_t dir);




int main(void) {
	inits();
	filestuff();
	while (1){
	f3d_nunchuk_read(&nundata);
	// Constrain button boolean to value of 0 or 1
	button = button % 3;
	if (button == 0){
	  draw(image[0], get_direction());
	} 
	if (button == 1){
	  draw(image[1], get_direction());
	}
	if (button == 2) {
	  draw(image[2], get_direction());
	}
	if ((nundata.z == 1) || (nundata.jx == 255)) {
	  button++;
	  f3d_lcd_fillScreen(WHITE);
	}
	if ((nundata.c == 1) || (nundata.jx == 0)) {
	  button--;
	  f3d_lcd_fillScreen(WHITE);
	  f3d_led_all_off();
	  if (button < 0) button += 3;       // Check for negative button value
	}	      
	}
}


// calculate the direction
uint8_t get_direction(){
  //Get data 
  f3d_accel_read(accel_data);
  //calculate pitch and roll from lab 8
  pitch = atanf(accel_data[0] / sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2)));
  roll = atanf(accel_data[1] / sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2)));
  printf("%f %f\n", pitch, roll); 
  //conditions
  if(pitch > 0.5){
    direction = 0x4;
    if(orientation != 1){
      orientation = 1;
      f3d_lcd_fillScreen(BLACK);//done so when rotation occurs, the blank space is white;
    }
  } else if(pitch < -0.5){
    direction = 0x2; 
    if(orientation != 2){
      orientation = 2;
      f3d_lcd_fillScreen(BLACK);//done so when rotation occurs, the blank space is white;
    }
  } else if(roll > 0.5){
    direction = 0x7;
    if(orientation != 3){
      orientation  = 3;
      f3d_lcd_fillScreen(BLACK);//done so when rotation occurs, the blank space is white;
    }
  } else if (roll < -0.5){
    direction = 0x1;
    if(orientation != 4){
      orientation  = 4;
      f3d_lcd_fillScreen(BLACK);//done so when rotation occurs, the blank space is white;

    }
  }
  return direction;
}


//draw an image
void draw(char filename[], uint8_t dir){
	int i ;
	int j ;
	uint16_t color;
	int h = 0;
	int w = 0;
	rc = f_open(&Fil, filename, FA_READ);
	if(rc) die(rc);

	//Reading headers from bmp files
	UINT br;
	f_read(&Fil, (void *) &magic, 2, &br);
	f_read(&Fil, (void *) &header, sizeof(header), &br);
	f_read(&Fil, (void *) &info, sizeof(info), &br);
	//remove bits so images lines up  
	const int TRASH = sizeof(bmp) * 28;
	bmp bmpTrash[TRASH];
	//Change h and w based on direction/orientation
	if (dir == 0x7 || dir == 0x1){ //Landscape
		h = 128;
		w = 128;
	}
	else{ //portrait
		h = 160;
		w = 128;
	}
	for(i = 0; i < h; i++){
		rc = f_read(&Fil, Imgbuf, sizeof Imgbuf, &br);
		if(rc) die(rc);
		f3d_lcd_setAddrWindow(0, i, 127, i, dir);
		//Bit shifting for R,G,B values	
		for(j = 0; j < w; j++){
		  //printf("%d\n" , j);
		  color = ((uint16_t) (Imgbuf[j].r >> 3)) << 11 | ((uint16_t) (Imgbuf[j].g >> 2)) << 5 | ((uint16_t) (Imgbuf[j].b >> 3));
			//f3d_lcd_drawPixel(j, i, color);
		  clr[j] = color;
		}
		f3d_lcd_pushColor(clr,128);
	}
	rc = f_close(&Fil);
	if(rc) die(rc);
}


//Uses nunchuck z and c button data history to select the picture in the sequence
void inits(void){
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	f3d_delay_init();
	f3d_lcd_init();
	f3d_uart_init();
	f3d_rtc_init();
	f3d_i2c1_init();
	f3d_accel_init();
	f3d_nunchuk_init();
}
void filestuff(void){
	char footer[20];
	int count=0;
	int i;
	FRESULT rc;			/* Result code */
	DIR dir;			/* Directory object */
	FILINFO fno;			/* File information object */
	UINT bw, br;
	unsigned int retval;

	f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

	printf("\nOpen an existing file (message.txt).\n");
	rc = f_open(&Fil, "MESSAGE.TXT", FA_READ);
	if (rc) die(rc);
	printf("\nType the file content.\n");
	for (;;) {
		rc = f_read(&Fil, Buff, sizeof Buff, &br);	/* Read a chunk of file */
		if (rc || !br) break;			/* Error or end of file */
		for (i = 0; i < br; i++)		        /* Type the data */
			putchar(Buff[i]);
	}
	if (rc) die(rc);
	
	printf("\nClose the file.\n");
	rc = f_close(&Fil);
	if (rc) die(rc);

	printf("\nCreate a new file (hello.txt).\n");
	rc = f_open(&Fil, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
	if (rc) die(rc);

	printf("\nWrite a text data. (Hello world!)\n");
	rc = f_write(&Fil, "Hello world!\r\n", 14, &bw);
	if (rc) die(rc);
	printf("%u bytes written.\n", bw);

	printf("\nClose the file.\n");
	rc = f_close(&Fil);
	if (rc) die(rc);

	printf("\nOpen root directory.\n");
	rc = f_opendir(&dir, "");
	if (rc) die(rc);

	printf("\nDirectory listing...\n");
	for (;;) {
		rc = f_readdir(&dir, &fno);		/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR)
			printf("   <dir>  %s\n", fno.fname);
		else
			printf("%8lu  %s\n", fno.fsize, fno.fname);
	}
	if (rc) die(rc);

	printf("\nTest completed.\n");

	rc = disk_ioctl(0,GET_SECTOR_COUNT,&retval);
	printf("%d %d\n",rc,retval);
}



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

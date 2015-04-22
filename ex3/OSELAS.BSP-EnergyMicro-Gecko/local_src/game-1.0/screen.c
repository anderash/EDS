#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#include "screen.h"
struct fb_copyarea rect;

uint16_t *screen_value;

int fb;


void initScreen() {
	rect.dx = 0;
	rect.dy = 0;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;	

	fb = open("/dev/fb0", O_RDWR);
	if (!fb){
		printf("Failed to open file /dev/fb0 \n");
		return;
	}
	screen_value = (uint16_t*)mmap(NULL, SCREEN_WIDTH * SCREEN_HEIGHT * 2, PROT_WRITE, MAP_SHARED, fb, 0);
	if (screen_value == MAP_FAILED){
		printf("Failed to map the screen\n");
	}
	refreshScreen();

}

void refreshScreen(){
	ioctl(fb, 0x4680, &rect);
}

void clearScreen(int color){
	int i;
	for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i){
		screen_value[i] = color;
	}
	refreshScreen();
}

void drawLine (int x, int y, int color) {
	int i;
	for (i = 0; i < x; ++i)
	{
		screen_value[i + y * SCREEN_WIDTH] = color;
	}
	refreshScreen();
}



void drawRect(int x, int y, int color){
	int i;
	int j;
for(i = x; i < x + 10; i++){
	for(j = y; j < y + 10; j++){
		screen_value[i + j * SCREEN_WIDTH] = color;
		}
	}
	
}

void drawBall(int x, int y, int r, int color){

}

void drawText(char *matrix){
int color = 34;
int i;
int j;
	for(i = 0; i < 32; i++){
		for(j = 0; j < 24; j++){
			switch(matrix[i + j * 32]){
				case 'X':
				color = col_black;
				break;
				case 'Y':
				color = col_yellow;
				break;
				case 'G':
				color = col_green;
				break;
				case 'B':
				color = col_blue;
				break;
				case 'R':
				color = col_red;
				break;
				case 'W':
				color = col_white;
				default:
				break;
			}
		drawRect(i*10, j*10, color);
		}
	}
refreshScreen();
}

void drawScore(char *matrix, int X, int Y){
	int color=0;
	int i;
	int j;
	for(i=0; i<3; i++){
		for(j=0; j<8; j++){
			switch(matrix[i + j * 3]){
				case 'W':
				color =col_white;
				break;
				case 'X':
				color = col_black;
				break;
			}
		drawRect((i+X)*10, (j+Y)*10, color);
		}
	}
	refreshScreen();
}

#include <stdint.h>
#include <stdio.h>


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define col_red 0xF000
#define col_green 0x07E0
#define col_blue 0x00FF
#define col_yellow 0xFF00
#define col_white 0xFFFF
#define col_black 0x0000

void initScreen();
void refreshScreen();
void clearScreen(int color);
void drawLine(int x, int y, int color);
void drawRect(int x, int y, int color);
void drawText(char *text);
void drawScore(char *score, int X, int Y);
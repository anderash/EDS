#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <math.h>
#include "screen.h"
#include "text.h"

void guessGame(), scoreDraw(), newScore();
int button, tries, maxGuess, target, num;
char scoreArr[8];
int new_gamepad();
void sigio_handler(int sig_in);
int input_func(int input);
bool run;
FILE *gamepad;

int main(int argc, char *argv[])
{
printf("Hello World, I'm game!\n");
printf("The game is on... Havlov91 won! AndyA7 and FreddyBoy lost\n");
printf("Endring på skjerm: \n");

new_gamepad();
initScreen();

//	drawRect(0,0,0xF000);
drawLine(100, 100, 0xF000);

drawText(pongIntro);
//printf("Welcome!\n");
//sleep(3);
clearScreen(col_black);
guessGame();
run = true;
while(run == true){
	if(button == 1){
		//printf("Knapp 1 er trykket bitches!\n");
		button = 1;
		newScore();
		button = 0;
	}
	else if(button == 2){
		//printf("Knapp 2 er trykket bitches!\n");
		newScore();
		button = 0;
	}	
	else if(button == 3){
		//printf("Knapp 3 er trykket bitches!\n");
		//button = 3;
		newScore();
		button = 0;
	}
	else if(button == 4){
		//printf("Knapp 4 er trykket bitches!\n");
		newScore();
		button = 0;
	}
	else if(button == 5){
		//printf("Knapp 5 er trykket bitches!\n");
		newScore();
		button = 0;
	}	
	else if(button == 6){
		//printf("Knapp 6 er trykket bitches!\n");
		newScore();
		button = 0;
	}		
	else if(button == 7){
		//printf("Knapp 6 er trykket bitches!\n");
		newScore();
		button = 0;
	}	
	else if(button == 8){
		//printf("Knapp 6 er trykket bitches!\n");
		newScore();
		button = 0;
	}	
}	
exit(EXIT_SUCCESS);
}
void newScore(){
	clearScreen(col_black);
	if (tries == maxGuess){
		printf("Out of tries, you loose\n");
		button = 0;
		scoreDraw();
		sleep(4);
		run = false;
		return;
	}
	target = (rand() % 8) + 0.5; //random number from 1 to 8
	printf("target = \n", (char)target);
	if (button == target) {
		
		printf("You won\n");
		drawText(win);
		sleep(1);
		clearScreen(col_green);
		num++;
		button = 0;
		//return;
	}
	else{
	clearScreen(col_red);
	}
	tries++;
}
void scoreDraw(){
	
	switch (num){
		case 1:
			drawScore(one,10,10);
			break;
		case 2:
			drawScore(two,10,10);
			break;
		case 3:
			drawScore(three,10,10);
			break;
		case 4:
			drawScore(four,10,10);
			break;
		case 5:
			drawScore(five,10,10);
			break;
		case 6:
			drawScore(six,10,10);
			break;
		case 7:
			drawScore(seven,10,10);
			break;
	}
}
void guessGame(){
	num = 0;	
	button = 0;
	tries = 0;
	maxGuess = 16;
	target = 0;
	printf("I want to play a game..\n");
	printf("Pick a button. Make a choice, live or die..\n");
}

int new_gamepad(){
	gamepad = fopen("/dev/gamepad", "rb");
	if(!gamepad){
		printf("Couldn't open gamepad driver\n");
		return EXIT_FAILURE;
	}
	if(fcntl(fileno(gamepad), F_SETOWN, getpid()) == -1) {
		printf("Error setting owner\n");
		return EXIT_FAILURE;
	}
	if(signal(SIGIO, &sigio_handler) == SIG_ERR){
		printf("Errer while register a signal handler.\n");
		return EXIT_FAILURE;
	}
	long flags = fcntl(fileno(gamepad), F_GETFL);
	if (fcntl(fileno(gamepad), F_SETFL, flags|FASYNC) == -1){
		printf("Error setting sync-flag\n");
		return EXIT_FAILURE;
	}
	printf("The gamepad is connected\n");
	return EXIT_SUCCESS;	
}

void sigio_handler(int sig_in){
	int input = input_func(fgetc(gamepad));
	
	switch (input) {
		case 1:
			button = 1;
			break;
		case 2:
			button = 2;
			break;
		case 3:
			button = 3;
			break;
		case 4:
			button = 4;
			break;
		case 5:
			button = 5;
			break;
		case 6:
			button = 6;
			break;
		case 7:
			button = 7;
			//run = false;
			break;
		case 8:
			button = 8;
			//guessGame();
			break;
	}
}
int input_func(int input){
	input = ~input;
	int i = 0;
	for ( i = 0; i < 8; i++) {
		int match = input & (1 << i);
		if ( (1 << i) == match ) {
			return (i+1);
		}
	}
return 0;
}	


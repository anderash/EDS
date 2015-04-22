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

void resetGame(), scoreDraw(int num), guessGame(int button);
int button, tries, maxGuess, target, score, high_score;
int new_gamepad();
void sigio_handler(int sig_in);
int input_func(int input);
bool run;
FILE *gamepad;

int main(int argc, char *argv[])
{
printf("Hello World, I'm game!\n");

new_gamepad();
initScreen();

//	drawRect(0,0,0xF000);
drawLine(100, 100, 0xF000);

drawText(pongIntro);
//printf("Welcome!\n");
//sleep(3);
clearScreen(col_black);
resetGame();
run = true;
high_score = 0;

	while(run){
		pause();	
	}
clearScreen(col_black);	

exit(EXIT_SUCCESS);
}

void scoreDraw(int num){
	clearScreen(col_black);
	drawScore(letter_h,0,0);	
	switch (high_score){
		case 1:
			drawScore(one,0,10);
			break;
		case 2:
			drawScore(two,0,10);
			break;
		case 3:
			drawScore(three,0,10);
			break;
		case 4:
			drawScore(four,0,10);
			break;
		case 5:
			drawScore(five,0,10);
			break;
		case 6:
			drawScore(six,0,10);
			break;
		case 7:
			drawScore(seven,0,10);
			break;
	}
	drawScore(letter_u,10,0);
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

void resetGame(){
	score = 0;	
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
		printf("Error while register a signal handler.\n");
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
	button = input;
	if (tries == maxGuess){
		if (input == 1){
			if (score > high_score){
				high_score = score;
			}
			run = true;
			resetGame();
		}
		else{
			run = false;
			return;
		}		
		
	}
	else{
		guessGame(input);
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

void guessGame(int button){

	clearScreen(col_black);
	
	target = (rand() % 9) + 0.5; //random number from 1 to 8
	printf("target = %d \n", target);
	if (button == target) {		
		printf("You guessed correct\n");
		drawText(win);
		sleep(1);
		clearScreen(col_green);
		score++;
		button = 0;
		//return;
	}
	else{
		clearScreen(col_red);
		sleep(1);
	}
	tries++;
	if (tries == maxGuess){
		printf("Out of tries, press 1 to try again\n");
		scoreDraw(score);
		sleep(3);
	}
	
}
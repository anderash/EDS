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
int tries, maxGuess, target, score, high_score;
int new_gamepad();
void sigio_handler(int sig_in);
int input_func(int input);
bool run;
FILE *gamepad;

int main(int argc, char *argv[])
{
printf("Hello World, I'm the guessGame!\n");

new_gamepad();
initScreen();

drawText(guess_intro);

sleep(3);
clearScreen(col_black);
resetGame();
run = true;
high_score = 0;

	while(run){
		pause();	
	}
	

exit(EXIT_SUCCESS);
}

void resetGame(){
	score = 0;	
	button = 0;
	tries = 0;
	maxGuess = 16;
	target = 0;
	printf("I want to play a game..\n");
	printf("Pick a button. Make a choice, live or die..\n");
	drawText(saw_face);
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
	printf("Button pressed: %d \n", input);
	if (input == 0){
		return;
	}
	if (tries == maxGuess){
		if (score > high_score){
				sleep(1);
				high_score = score;
				drawText(win);
				sleep(1);
			}
		if (input == 1){
			
			run = true;
			resetGame();
		}
		else if (input == 6 && high_score >=4){
			printf("NOOO! You found the way out.. And pressed the magic button\n");
			drawText(win);
			run = false;
		}
		else{	
			printf("U have no choise..\n");
			resetGame();
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
	
	target = (rand() % 8) + 1; //random number from 1 to 8
	printf("target = %d \n", target);
	if (button == target) {		
		printf("You guessed correct\n");
		drawText(win);
		sleep(1);
		clearScreen(col_green);
		score++;
		button = 0;
	}
	else{
		clearScreen(col_red);
		drawText(saw_face);
	}
	tries++;
	if (tries == maxGuess){
		printf("Out of tries, press 1 to try again\n");
		drawText(saw_face);
		scoreDraw(score);				
	}
	
}

void scoreDraw(int num){
	drawScore(letter_h,0,0);
	drawScore(one, 3,0);
	drawScore(letter_col,6,0);	
	switch (high_score){
		case 0:
			drawScore(zero,10,0);
			break;
		case 1:
			drawScore(one,10,0);
			break;
		case 2:
			drawScore(two,10,0);
			break;
		case 3:
			drawScore(three,10,0);
			break;
		case 4:
			drawScore(four,10,0);
			break;
		case 5:
			drawScore(five,10,0);
			break;
		case 6:
			drawScore(six,10,0);
			break;
		case 7:
			drawScore(seven,10,0);
			break;
	}
	drawScore(letter_u,0,10);
	drawScore(letter_col,6,10);
	switch (num){
		case 0:
			drawScore(zero,10,10);
			break;
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
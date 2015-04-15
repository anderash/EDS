#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <unistd.h>

#include "screen.h"
#include "text.h"

void guessGame();

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	printf("The game is on... AndyA7 won! Havlov91 and FreddyBoy lost\n");
	printf("Endring på skjerm: \n");


	initScreen();

//	drawRect(0,0,0xF000);
	drawLine(100, 100, 0xF000);

	drawText(pongIntro);
	printf("Welcome!\n");
	sleep(3);
	clearScreen(col_black);
	guessGame();
	exit(EXIT_SUCCESS);
}


void guessGame(){
	int button;
	int target;
	int tries = 0;
	int maxGuess = 8;
	time_t t;
	srand((unsigned)time(&t));
	while (1){
		//get button press
		//Set button press to int
		button = rand()%8;

		target = rand()%8;
		if (button == target) {
			printf("You won\n");
			drawText(win);
			return;
		}
		else if (tries == maxGuess){
			printf("Out of tries, you loose\n");
			return;
		}
		else{
			switch (tries){
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
		tries++;
		sleep(2);
	}
}

void 
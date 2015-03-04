#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


// Funtion to play TADA
bool playTada(int cnt)
{
	*DAC0_CH0DATA = Tada[cnt];
	*DAC0_CH1DATA = Tada[cnt];
	bool finished = false;

	if (cnt > 28600) {		
		finished = true;
	}
	else{		
		finished = false;
	}
	return finished;
}

// Funtion to play Error
bool playError(int cnt)
{
	*DAC0_CH0DATA = Error[cnt];
	*DAC0_CH1DATA = Error[cnt];
	bool finished = false;

	if (cnt > 12000) {		
		finished = true;
	}
	else{		
		finished = false;
	}
	return finished;
}

// Funtion to play marioJump
bool playMariojump(int cnt)
{
	*DAC0_CH0DATA = marioJump[cnt];
	*DAC0_CH1DATA = marioJump[cnt];
	bool finished = false;

	if (cnt > 8830) {		
		finished = true;
	}
	else{		
		finished = false;
	}
	return finished;
}

//Function to choose piano-tones and return correct period for sawtooth
int chooseTone(uint32_t button)
{
	int period;
	switch (button){
		case SW1:
			period = SAMPLE_F/NOTE_C4;
			break;
		case SW2:
			period = SAMPLE_F/NOTE_D4;
			break;
		case SW3:
			period = SAMPLE_F/NOTE_E4;
			break;
		case SW4:
			period = SAMPLE_F/NOTE_F4;
			break;
		case SW5:
			period = SAMPLE_F/NOTE_G4;
			break;
		case SW6:
			period = SAMPLE_F/NOTE_A4;
			break;
		case SW7:
			period = SAMPLE_F/NOTE_B4;
			break;
		case SW8:
			period = SAMPLE_F/NOTE_C5;
			break;
		default:
			period = 50;


	}
	return period;

}



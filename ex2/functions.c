#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


// Funtion to play TADA
bool playTada(int cnt)
{
	*DAC0_CH0DATA = Tada[cnt];
	*DAC0_CH1DATA = Tada[cnt];
	bool finished = false;

	if (cnt > 105507) {		
		finished = true;
	}
	else{		
		finished = false;
	}
	return finished;
}

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
//Funtion to choose piano-tones and return correct period for sawtooth
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
			period = 73;


	}
	return period;

}


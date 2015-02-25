#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

uint32_t LISA[] = {SW1, 0xFF, SW2, 0xFF, SW3, 0xFF, SW4, 0xFF, SW5, SW5, 0xFF,SW5, SW5, 0xFF, SW6, 0xFF, SW6, 0xFF, SW6, 0xFF, SW6, 0xFF, SW5, SW5, 0xFF, SW4, 0xFF, SW4, 0xFF, SW4, 0xFF, SW4, 0xFF, SW3, SW3, 0xFF, SW3, SW3, 0xFF, SW2, 0xFF, SW2, 0xFF, SW2, 0xFF, SW2, 0xFF, SW1, SW1, SW1, 0xFF};

void setupDAC()
{
  /*
    TODO enable and set up the Digital-Analog Converter
    
    1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
    2. Prescale DAC clock by writing 0x500101 to DAC0_CTRL
    3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL
    4. Write a continuous stream of samples to the DAC data registers, 
    DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
  */
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;
    *DAC0_CTRL = 0x50010;
    *DAC0_CH0CTRL = 1;
    *DAC0_CH1CTRL = 1;

}

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


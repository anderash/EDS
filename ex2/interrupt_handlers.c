#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

//Variables used in interrupt handlers
int cnt1 = 0;
int lisaCnt = 0;
int delay1 = 0;
int freq = 73;
bool DACon = true;
bool finished = false;
bool startup = true;
uint32_t note;
uint32_t knapp;
uint32_t lastPress;
uint32_t lights;
uint8_t incrementValue;
uint8_t lydniva = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  


*TIMER1_IFC = 1;

knapp = *GPIO_PC_DIN;

//PLAY Statup-tada when the chip boots
if(startup && !finished){
	*GPIO_PA_DOUT = 0x00 << 8;
	finished = playTada(cnt1);
	if (finished){
		cnt1 = 0;
		startup = false;
	}
	cnt1 = cnt1 + 1;
}
	
//Songs not finished playing
else if (!finished){

	switch(lastPress){

		//PLAY "Tada"			
		case SW1:
			*GPIO_PA_DOUT = SW1 << 8;	//Set LED
			finished = playTada(cnt1);	//Use playfunc.
			if (finished){				//Done playing, reset
				cnt1 = 0;
			}
			cnt1 = cnt1 + 1;			
			break;

		// PLAY "LISA GIKK TIL SKOLEN"	
		case SW2:		
			note = LISA[lisaCnt];			//Chose note from note array
			lights = note << 8;				//Set LED
			*GPIO_PA_DOUT = lights; 
			freq = chooseTone(note);		//Choose correct freq. for that note
			finished = false;
			incrementValue = 0xFF/(freq);	//Set the incremention rate for the sawtooth

			// Sawtooth wave
			if(note <= 0xFE){				//Make shure it is not a "brake" in the song
				if (cnt1 < freq){					
					*DAC0_CH0DATA = lydniva;
					*DAC0_CH1DATA = lydniva;
					lydniva = lydniva + incrementValue;
				}
				else{
					lydniva = 0;
					cnt1 = 0;			
				}
			}

			cnt1 = cnt1 + 1;
			delay1 = delay1 + 1;	

			if(delay1 > 2756){				//Each note is held for 2756 counts = 0.125 sec
				lisaCnt = lisaCnt + 1;
				delay1 = 0;
				if(lisaCnt > 50){			//Song is done: reset counters
					lisaCnt = 0;
					cnt1 = 0;
					finished = true;		
				}
			}
			break;

		//PLAY Error sound
		case SW3:
			*GPIO_PA_DOUT = SW3 << 8;
			finished = playError(cnt1);
			if (finished){
				cnt1 = 0;
			}
			cnt1 = cnt1 + 1;
			break;

		//PLAY Mariojump
		case SW4:
			*GPIO_PA_DOUT = SW4 << 8;
			finished = playMariojump(cnt1);
			if (finished){
				cnt1 = 0;
			}
			cnt1 = cnt1 + 1;
			break;

		//ENABLE PIANO
		case SW5:
			*GPIO_IEN = 0;	//Disable GPIO interrupts to play piano

			if (knapp <= 0xFE){			//Make shure a button is pressed
				freq = chooseTone(knapp);
				incrementValue = 0xFF/(freq);
				lights = knapp << 8;
				*GPIO_PA_DOUT = lights;	
				if (knapp == SW8){		//Disable piano by pressing SW8
					*GPIO_IEN = 0xFF;
					finished = true;
					break;
				}
				if (cnt1 < freq){					
					*DAC0_CH0DATA = lydniva;
					*DAC0_CH1DATA = lydniva;
					lydniva = lydniva + incrementValue;
					cnt1 = cnt1 + 1;
				}
				else{
					lydniva = 0;
					cnt1 = 0;			
				}

			}
			else{						//No button pressed, turn off lights
				lights = knapp << 8;
				*GPIO_PA_DOUT = lights;
			}
			
			finished = false;
			break;


		default:
			finished = true;
			cnt1 = 0;
	}
}

//Nothing is playing: reset variables, disable DAC and go to deep sleep
else{
	lights = NO_SW << 8;
	*GPIO_PA_DOUT = lights;
	cnt1 = 0;
	*DAC0_CH0DATA = 0x80;
	*DAC0_CH1DATA = 0x80;
	lastPress = NO_SW;
	if(DACon){
		disableDAC();
		DACon = false;
	}
	*SCR = 6; 				//ENABLE DEEP SLEEP WHEN NOT PLAYING ANYTHING

}


}
    



void GPIO_HANDLER()
{

	*GPIO_IFC = 0xff;
	knapp = *GPIO_PC_DIN;
	lastPress = knapp;
	finished = false;
	cnt1 = 0;
	lisaCnt = 0;
	if (!DACon){
		setupDAC();
		DACon = true;
	}

	*SCR = 2;			//DISABLE DEEP SLEEP, BUT ENABLE SLEEP
	
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
		GPIO_HANDLER();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
		
		GPIO_HANDLER();
}



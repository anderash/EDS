#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


int cnt1 = 0;
int cnt2 = 0;
int lisaCnt = 0;
int delay = 0;
int freq = 73;
bool finished = true;
uint32_t note;
uint32_t knapp;
uint32_t lastPress;
uint32_t lights;
uint8_t incrementValue;
uint8_t lydniva = 0;
/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */ 
	*TIMER1_IFC = 1;

knapp = *GPIO_PC_DIN;


//Only play when a button is pressed
if(knapp <= 0xFE){ 
	lights = knapp << 8;
	*GPIO_PA_DOUT = lights;

	switch (knapp){
		case SW1:
			lastPress = knapp;
			finished = false;
			break;
		case SW2:
			lastPress = knapp;
			finished = false;
			break;
		case SW3:
			lastPress = knapp;
			finished = false;
			break;
		case SW4:
			
			break;
		case SW5:
			
			break;
		case SW6:
			
			break;
		case SW7:
			
			break;
		case SW8:
			
			break;
		default:
			lastPress = 0xFF;
	
	}

}

if (!finished){
	switch(lastPress){
		//PLAY "Tada"
		case SW1:
			finished = playTada(cnt1);
			if (finished){
				cnt1 = 0;
			}
			cnt1 = cnt1 + 1;
			break;

		// PLAY "LISA GIKK TIL SKOLEN"	
		case SW2:		
			note = LISA[lisaCnt];
			lights = note << 8;
			*GPIO_PA_DOUT = lights; 
			freq = chooseTone(note);
			incrementValue = 0xFF/(2*freq);				
			finished = false;

			// Sawtooth wave
				if (cnt1 < 2*freq){					
					*DAC0_CH0DATA = lydniva;
					*DAC0_CH1DATA = lydniva;
					lydniva = lydniva + incrementValue;
				}
				else{
					lydniva = 0;
					cnt1 = 0;			
				}

				cnt1 = cnt1 + 1;
				delay = delay + 1;	

				if(delay > 2000){
					lisaCnt = lisaCnt + 1;
					delay = 0;
					if(lisaCnt > 50){
						lisaCnt = 0;
						finished = true;		
					}
				}
			break;

		case SW3:
			finished = playError(cnt1);
			if (finished){
				cnt1 = 0;
			}
			cnt1 = cnt1 + 1;
			break;
		default:
			finished = true;
			cnt1 = 0;
	}
}
else{
	lights = knapp << 8;
	*GPIO_PA_DOUT = lights;
	cnt1 = 0;
	*DAC0_CH0DATA = 0x80;
	*DAC0_CH1DATA = 0x80;
}


	
	// variables for piano

}
    



void GPIO_HANDLER()
{
		*GPIO_IFC = 0xff;
	  uint32_t a = *GPIO_PC_DIN;
 	  a = a << 8;
		*GPIO_PA_DOUT = a; 
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
		*GPIO_IFC = 0xff;
		//GPIO_HANDLER();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
		
		//GPIO_HANDLER();
}



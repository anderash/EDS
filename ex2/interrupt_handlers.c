#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "efm32gg.h"


int cnt1 = 0;
int cnt2 = 0;
int lisaCnt = 0;
int delay1 = 0;
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

incrementValue = 0x3FF/(2*freq);		

if (!finished){

	switch(lastPress){

		//PLAY "Tada"			
		case SW1:
			*GPIO_PA_DOUT = SW1 << 8;
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
				delay1 = delay1 + 1;	

				if(delay1 > 2000){
					lisaCnt = lisaCnt + 1;
					delay1 = 0;
					if(lisaCnt > 50){
						lisaCnt = 0;
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

		case SW5:
			*GPIO_IEN = 0;
			if (knapp <= 0xFE){
				freq = chooseTone(knapp);
				if (knapp == SW8){
					*GPIO_IEN = 0xFF;
					finished = true;
					break;
				}
				if (cnt1 < 2*freq){					
					*DAC0_CH0DATA = lydniva;
					*DAC0_CH1DATA = lydniva;
					lydniva = lydniva + incrementValue;
				}
				else{
					lydniva = 0;
					cnt1 = 0;			
				}

			}
			
			finished = false;
			break;

		case SW8:
			finished = true;
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
	lastPress = 0xFF;

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
	
	


	  //uint32_t a = *GPIO_PC_DIN;
	  
 	  //a = a << 8;
		//*GPIO_PA_DOUT = a; 
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



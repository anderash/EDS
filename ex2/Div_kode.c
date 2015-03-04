// DIV KODE

	//TELLESHITT I INTERRUPT
	/*
	cnt1 = cnt1 + 1;
	if(cnt1 == 20000){
		cnt1 = 0;
		lights = *GPIO_PA_DOUT;
		if(lights < 0xfdff){
			lights = lights + 256;
			*GPIO_PA_DOUT = lights;
		}
		else {
			lights = 0x0000;
			*GPIO_PA_DOUT = lights;
		}	
	}*/
		
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
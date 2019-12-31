#include "ADC.h"

uint32_t ADC_Last_Light_Left = 0;
uint32_t ADC_Last_Light_Right = 0;
uint32_t ADC_Last_Speed = 0;
uint8_t ADC_New_Left_Light_Available = 0;
uint8_t ADC_New_Right_Light_Available = 0;
uint8_t ADC_New_Speed_Available = 0;

void ADC_Timer_Init() {
	TIMER_PIN_IOCON &= ~(0x07);
	TIMER_PIN_IOCON |= 0x03;
	
	PCONP |= 1 << 1;
	
	TIMER0->CTCR = 0x0;
	
	TIMER0->TCR &= ~(1 << 0);
	
	TIMER0->TCR |= (1 << 1);
	
	TIMER0->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000000 - 1;
	
	//Toggle the corresponding External Match bit/output when MR1 matches with TC.
	TIMER0->EMR |= (1<<6 | 1<<7);
	//Reset the TC value whenever MR1 matches it
	TIMER0->MCR |= 1<<4;
	//Configure MR1 as ADC will start every 100 milliseconds (Do not forget you configured ADC when rising edge occurs on TIMER 0 MATCH 1) 
	TIMER0->MR1 = 100000;
	
	TIMER0->TCR &= ~(1 << 1);
	
	TIMER0->TCR |= (1 << 0);
}

void ADC_Init() {
	uint32_t temp;
	ADC_Timer_Init(); // Initialize timer 0 for adc 
	
	//Change the function value of pin to ADC.
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	//Change Analog/Digital mode of pin to Analog.
		
	temp = LEFT_LDR_ANALOG_PIN_IOCON;
	temp |= 1<<0;
	temp &= ~(1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<7);
	LEFT_LDR_ANALOG_PIN_IOCON = temp;
	
	temp = RIGHT_LDR_ANALOG_PIN_IOCON;
	temp |= 1<<0;
	temp &= ~(1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<7);
	RIGHT_LDR_ANALOG_PIN_IOCON = temp;
	
	temp = POTENTIOMETER_ANALOG_PIN_IOCON;
	temp |= (1<<0 | 1<<1);
	temp &= ~(1<<2 | 1<<3 | 1<<4 | 1<<7);
	POTENTIOMETER_ANALOG_PIN_IOCON = temp;
	
	//Turn on ADC.
	PCONP |= 1<<12; 
	
	//Make the A/D converter operational
	//Make sure conversions are software controlled and require 31 clocks (Do not use Burst mode)
	//Set the CLKDIV and make the A/D converter operational without Burst mode.
	//Configure CR SEL bits for sampled and converting corresponding pin.
	//Set start bits 000
	temp = ADC->CR;
	temp &= ~(1<<0 | 1<<1 | 1<<3 | 1<<6 | 1<<7 | 1<<10 | 1<<14 | 1<<15 | 1<<24 | 1<<25 | 1<<26);
	temp |= (1<<2 | 1<<4 | 1<<5 | 1<<8 | 1<<9 | 1<<11 | 1<<12 | 1<<13 | 1<<16 | 1<<21);
	ADC->CR = temp;
	
	//Enable interrupt for corresponding pin.
	ADC->INTEN |= 1<<2;
	//Enable ADC_IRQn (Interrupt Request).
	NVIC_EnableIRQ(ADC_IRQn);
}

/*
void ADC_Start () {
	uint32_t temp;
	temp = ADC->CR;
	temp |= (1<<26);
	temp &= ~(1<<24 | 1<<25);
	ADC->CR = temp;
}
*/
uint32_t ADC_Get_Last_Left_Light() {
	ADC_New_Left_Light_Available = 0;
	return ADC_Last_Light_Left;
}
uint32_t ADC_Get_Last_Right_Light() {
	ADC_New_Right_Light_Available = 0;
	return ADC_Last_Light_Right;
}
uint32_t ADC_Get_Last_Speed() {
	ADC_New_Speed_Available = 0;
	return ADC_Last_Speed;
}

void ADC_IRQHandler() {
	uint8_t channel;
	uint32_t conversionStatus, mask;
	conversionStatus = ADC->GDR;
	ADC->GDR &= ~((uint32_t)1 << 31);
	mask = (1<<24 | 1<<25 | 1<<26);
	channel = ((conversionStatus & mask) >> 24);
	if(channel == 2){ // left ldr
			ADC_Last_Light_Left = ((ADC->DR[2] << 16)>>20);
			ADC_New_Left_Light_Available = 1;
	}else if(channel == 4){ // potentiometer
			ADC_Last_Speed = ((ADC->DR[4] << 16)>>20);
			ADC_New_Speed_Available = 1;
	}else if(channel == 5){ // right ldr
			ADC_Last_Light_Right = ((ADC->DR[5] << 16)>>20);
			ADC_New_Right_Light_Available = 1;
	}
}

#include "Timer.h"
#include "PWM.h"

void Timer1_Init() {
	/*
	PCONP |= 1 << 2;
	
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_SetPriority(TIMER1_IRQn, 7);
	
	// enable timer
	TIMER1->TCR &= ~(1 << 0);	
	TIMER1->TCR |= (1 << 1);
	
	// timer mode on
	TIMER1->CTCR &= ~(3);
	
	// increments timer counter on every 0.25 seconds
	TIMER1->PR = 10000;
	
	// enables interrupt on MR3 match and resets it
	TIMER1->MCR |= 3 << 9;
	
	// TODO: comment it
	TIMER1->MR3 = 2;
	//*/
	
}

void TIMER1_IRQHandler() {
	TIMER1->IR |= 1 << 3;
	PWM_Write(0);
}

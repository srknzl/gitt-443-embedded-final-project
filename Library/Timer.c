#include "Timer.h"
#include "PWM.h"

void Timer1_Init() {
	///*
	PCONP |= 1 << 2;
	
	// enable timer
	TIMER1->TCR &= ~(1 << 1);	
	TIMER1->TCR |= (1 << 0);
	
	// timer mode on
	TIMER1->CTCR &= ~(1<<0 | 1<<1);
	
	// increments timer counter on every 0.25 seconds
	TIMER1->PR = 60000000 / 4 - 1;
	// TIMER1->EMR |= (1<<3 | 1<<10 | 1<<11);
	// enables interrupt on MR3 match and resets it
	TIMER1->MCR |= 3 << 9 | 3 << 10;
	
	// TODO: add comment
	//TIMER1->MR3 = 2;
	//*/
		
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_SetPriority(TIMER1_IRQn, 4);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	
}

void TIMER1_IRQHandler() {
	TIMER1->IR |= 1 << 3;
	PWM_ChangeDirection(2);
}

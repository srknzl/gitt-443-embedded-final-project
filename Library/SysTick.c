#include "SysTick.h"
#include "PWM.h"
#include "GPIO.h"

static uint32_t on = 1;

void SysTick_Init() {
	// enables the system tick counter and its interrupt
	// also sets CCLK as the clksource
	STCLK->SYCTRL |= 1<<0 | 1<<1 | 1<<2;
	
	// sets the systick to reset every 0.25 secs
	STCLK->STRELOAD = 12000000 - 1;
	
	// no external reference clock is unavailable
	STCLK->STCALIB = 1<<30 | 1<<31;
	
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 10);
	NVIC_ClearPendingIRQ(SysTick_IRQn);
}

void SysTick_Handler() {
	/*
	if(on) {
		LED1_On();
		on = 0;
	} else {
		LED1_Off();
		on = 1;
	}
	//*/
	update_LEDs();
}

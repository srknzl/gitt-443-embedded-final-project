#include "SysTick.h"
#include "PWM.h"
#include "GPIO.h"

static uint32_t on = 1;
/*
Desc: Init systick for blinking leds
*/
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
/*
Desc: Update leds states twice a second  
*/
void SysTick_Handler() {
	update_LEDs();
}

#include "Timer.h"
#include "SystemStructures.h"
void Timer0_Init() {
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

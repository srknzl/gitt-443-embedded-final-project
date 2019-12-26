#include "Ultrasonic.h"

uint32_t ultrasonicSensorRisingCaptureTime;
uint32_t ultrasonicSensorFallingCaptureTime;

uint32_t ultrasonicSensorDuration = 0;
uint32_t ultrasonicSensorDistance = 0;
uint8_t ultrasonicSensorNewDataAvailable = 0;

uint8_t ultrasonicSensorTriggerStart = 0;
uint8_t ultrasonicSensorCaptureRisingEdge = 0;

void Ultrasonic_Init() {
	IOCON_TRIGGER |= 0x03;
	IOCON_ECHO |= 0x03;
}

void Ultrasonic_Trigger_Timer_Init() {
	//Turn on Timer2.
	PCONP |= 1<<22;
	//Change the mode of Timer2 to Timer Mode.
	TIMER2->CTCR &= ~(1<<0 | 1<<1);
	TIMER2->TCR &= ~(1 << 0);
	
	TIMER2->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER2->PR = 59; // TC is incremented every 1 microseconds (10^-6)
	//Write the Correct Configuration for EMR (LOW output value of Trigger Pin when match occurs and Initial value is LOW)
	TIMER2->EMR |= (1<<3 | 1<<10); // Match pin is high initially, set low when match occurs.

	NVIC_EnableIRQ(TIMER2_IRQn);
	
	NVIC_SetPriority(TIMER2_IRQn,5);
	
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
}

void Ultrasonic_Capture_Timer_Init() {
	//Turn on Timer3
	PCONP |= 1<<23; 

	//Change the mode of Timer3 to Timer Mode
	TIMER3->CTCR &= ~(1<<0 | 1<<1);
	TIMER3->TCR &= ~(1 << 0);
	
	TIMER3->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = 59; // TC is incremented every 1 microseconds (10^-6)

	//Change CCR value for getting Interrupt when Rising Edge Occur for CAP 1
	TIMER3->CCR |= (1<<3 | 1<<4 | 1<<5); // Capture pin gives interrupt for rising and falling edges

	TIMER3->TCR &= ~(1 << 1);
	
	TIMER3->TCR |= (1 << 0);

	NVIC_EnableIRQ(TIMER3_IRQn);
}

void Ultrasonic_Start_Trigger_Timer() {
	//Change output value of Trigger Pin as HIGH
	GPIO_PIN_Write(PORT0, 1<<9, 1);
	//Give correct value to corresponding MR Register for 10 microsecond
	TIMER2->MR3 = 10;
	//Enable interrupt for MR3 register, if MR3 register matches the TC.
	TIMER2->MCR |= 1 <<9;
	//Remove the reset on counters of Timer2.
	TIMER2->TCR &= ~(1<<1); 
	//Enable Timer Counter and Prescale Counter for counting.
	TIMER2->TCR |= 1<<0;
}

void TIMER2_IRQHandler() {
	//Write HIGH bit value to IR Register for Corresponding Interrupt
	TIMER2->IR |= 1<<3;
	if(ultrasonicSensorTriggerStart == 0) {
		//Change MR3 Register Value for Suggested Waiting
		TIMER2->MR3 = 60000 + TIMER2->TC; // Suggested waiting is 60ms
		ultrasonicSensorTriggerStart = 1;
	}
	else {
		TIMER2->EMR |= (1 << 3);
		TIMER2->MR3 = 10 + TIMER2->TC;
		
		ultrasonicSensorTriggerStart = 0;
	}
}

void TIMER3_IRQHandler() {
	TIMER3->IR = 1 << 5;
	
	if(ultrasonicSensorCaptureRisingEdge == 1) {
		ultrasonicSensorRisingCaptureTime = TIMER3->CR1;
		
		LPC_TIM3->CCR = (1 << 4) | (1 << 5);
		ultrasonicSensorCaptureRisingEdge = 0;
	}
	else {
		ultrasonicSensorFallingCaptureTime = TIMER3->CR1;
		ultrasonicSensorNewDataAvailable = 1;
		
		LPC_TIM3->CCR = (1 << 3) | (1 << 5);
		ultrasonicSensorCaptureRisingEdge = 1;
	}
}


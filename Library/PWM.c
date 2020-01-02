#include "PWM.h"
#include "GPIO.h"
#include "DataStructures.h"
#include "SystemStructures.h"


void PWM_Init() {
	uint32_t  temp;
	PCONP |= (1 << 5 | 1 << 6);
	
	temp =  IOCON_MOTOR1_SPEED;
	temp |= 3;
	temp &= ~4;
	IOCON_MOTOR1_SPEED= temp;

	PWMX->PCR |= (1 << 11);
	PWMX->TCR = 1 << 1;
	PWMX->PR = 9;
	
	//Configure MR0 register for a period of 20 ms
	PWMX->MR0 = 120000;
	PWMX->MCR = 1 << 1;;
	
	PWMX->LER |= 1 << 0;
	
	PWMX->TCR = (1 << 0 | 1 << 3);
	
	//for(i=0;i<6000000;i++);
	//PWM_Write(0);
}

void PWM_Cycle_Rate(uint32_t period_In_Cycles) {
	//Write a formula that changes the MR0 register value for a given parameter.
	PWMX->MR0 = period_In_Cycles *6000;
	PWMX->LER |= 1 << 0;
}

void PWM_Write(uint32_t T_ON) {	
	
	int temp;
	if(T_ON > 100) {
		T_ON = 100;
	}
	temp = PWMX->MR0 * (100-T_ON)/100;
	//Write a formula to calculate the match register of the PWM pin.
	PWMX->MR3 = temp;
	
	//Enable PWM Match Register Latch.
	PWMX->LER |= 1 << 3;
}



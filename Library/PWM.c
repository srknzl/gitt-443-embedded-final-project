#include "PWM.h"
#include "GPIO.h"
#include "DataStructures.h"
#include "SystemStructures.h"


void PWM_Init() {
	uint32_t  temp;
	PCONP |= (1 << 5); // Turn on PWM0
	
	temp =  IOCON_MOTOR1_SPEED;
	temp |= (1<<0 | 1<<1);
	temp &= ~(1<<2);
	IOCON_MOTOR1_SPEED= temp;

	temp =  IOCON_MOTOR2_SPEED;
	temp |= (1<<0 | 1<<1);
	temp &= ~(1<<2);
	IOCON_MOTOR2_SPEED= temp;
	
	PWMX->PCR |= (1 << 11 | 1 << 12);  // Enable PWM0_3 and PWM0_4 output
	PWMX->TCR = 1 << 1; //The PWM Timer Counter and the PWM Prescale Counter are synchronously reset on the
											//next positive edge of PCLK. The counters remain reset until this bit is returned to zero
 
	PWMX->PR = 9;
	
	PWMX->MR0 = 120000;
	PWMX->MCR = 1 << 1;;
	
	PWMX->LER |= 1 << 0;
	
	PWMX->TCR = (1 << 0 | 1 << 3); // Start PWM counters
	
	//for(i=0;i<6000000;i++);
	//PWM_Write(0);
}

void PWM_Cycle_Rate(uint32_t period_In_Cycles) {
	//Write a formula that changes the MR0 register value for a given parameter.
	PWMX->MR0 = period_In_Cycles *6000;
	PWMX->LER |= 1 << 0;
}

void PWM_Write_Motor1(uint32_t T_ON) {	
	
	int temp;
	if(T_ON > 100) {
		T_ON = 100;
	}
	temp = PWMX->MR0 * (100-T_ON)/100;
	PWMX->MR3 = temp;
	
	//Enable PWM Match Register Latch.
	PWMX->LER |= 1 << 3;
}
void PWM_Write_Motor2(uint32_t T_ON) {	
	
	int temp;
	if(T_ON > 100) {
		T_ON = 100;
	}
	temp = PWMX->MR0 * (100-T_ON)/100;
	PWMX->MR4 = temp;
	
	//Enable PWM Match Register Latch.
	PWMX->LER |= 1 << 3;
}



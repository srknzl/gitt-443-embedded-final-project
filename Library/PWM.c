#include "PWM.h"
#include "GPIO.h"
#include "DataStructures.h"

/* For Motor controller:
 IN1 = P5
 IN2 = P6
 EN1 = P28
*/

void PWM_Init() {
	
	uint32_t  temp;
	
	//Change the function of the pin in here:
	PCONP |= (1 << 5 | 1 << 6);
	temp =  IOCON_MOTOR_SPEED;
	temp |= 3;
	temp &= ~4;
	IOCON_MOTOR_SPEED= temp;
	//Enable PWM output for corresponding pin.
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

void PWM_ChangeDirection(MotorDirection direction){
	if(direction == STOPMOVING){	
		GPIO_PIN_Write(PORT1,MC_IN1,1);
		GPIO_PIN_Write(PORT1,MC_IN2,1);
	} else if(direction == COUNTERCLOCKWISE){	
		GPIO_PIN_Write(PORT1,MC_IN1,0);
		GPIO_PIN_Write(PORT1,MC_IN2,1);
	}	else if(direction == CLOCKWISE){
		GPIO_PIN_Write(PORT1,MC_IN1,1);
		GPIO_PIN_Write(PORT1,MC_IN2,0);
	}
}

void Move_Forward(){
	// Todo add led functionality
	PWM_ChangeDirection(CLOCKWISE);
}
void Move_Backward(){
	// Todo add led functionality
	PWM_ChangeDirection(COUNTERCLOCKWISE);
}
void Turn_Left(){
	PWM_ChangeDirection(CLOCKWISE);
	// Todo add led functionality
	// Todo add turn left code
}
void Turn_Right(){
	PWM_ChangeDirection(COUNTERCLOCKWISE);
	// Todo add led functionality
	// Todo add turn left code
}
void Stop_Motors(){
	// Todo add led functionality
	PWM_ChangeDirection(STOPMOVING);
}
void Change_Speed(uint32_t speed){
	PWM_Write(100-speed);
}

#include "Motors.h"
#include "GPIO.h"
#include "PWM.h"

/* 
For Motor 1 controller:
IN1 = P5 -- P1_23
IN2 = P6 -- P1_24
EN1 = P28 -- P1_5 PWM0_3

For Motor 2 controller:

IN3 = P39 -- P5_0
IN4 = P38 -- P5_1
EN2 = P27 -- P1_6 PWM0_4

*/
void Motor1_ChangeDirection(MotorDirection direction){
	if(direction == STOPMOVING){	
		GPIO_PIN_Write(PORT1,MASK_IN1,0);
		GPIO_PIN_Write(PORT1,MASK_IN2,0);
	} else if(direction == COUNTERCLOCKWISE){	
		GPIO_PIN_Write(PORT1,MASK_IN1,0);
		GPIO_PIN_Write(PORT1,MASK_IN2,1);
	}	else if(direction == CLOCKWISE){
		GPIO_PIN_Write(PORT1,MASK_IN1,1);
		GPIO_PIN_Write(PORT1,MASK_IN2,0);
	}
}
void Motor2_ChangeDirection(MotorDirection direction){
	if(direction == STOPMOVING){	
		GPIO_PIN_Write(PORT5,MASK_IN3,0);
		GPIO_PIN_Write(PORT5,MASK_IN4,0);
	} else if(direction == COUNTERCLOCKWISE){	
		GPIO_PIN_Write(PORT5,MASK_IN3,0);
		GPIO_PIN_Write(PORT5,MASK_IN4,1);
	}	else if(direction == CLOCKWISE){
		GPIO_PIN_Write(PORT5,MASK_IN3,1);
		GPIO_PIN_Write(PORT5,MASK_IN4,0);
	}
}

void Move_Forward(){
	// Todo add led functionality
	Motor1_ChangeDirection(CLOCKWISE);
	Motor2_ChangeDirection(CLOCKWISE);
}
void Move_Backward(){
	// Todo add led functionality
	Motor1_ChangeDirection(COUNTERCLOCKWISE);
	Motor2_ChangeDirection(COUNTERCLOCKWISE);
}
void Turn_Left(){
	Motor1_ChangeDirection(CLOCKWISE);
	Motor2_ChangeDirection(COUNTERCLOCKWISE);
	// Todo add led functionality
}
void Turn_Right(){
	Motor1_ChangeDirection(COUNTERCLOCKWISE);
	Motor2_ChangeDirection(CLOCKWISE);
	// Todo add led functionality
}
void Stop_Motors(){
	// Todo add led functionality
	Motor1_ChangeDirection(STOPMOVING);
	Motor2_ChangeDirection(STOPMOVING);
}
void Change_Motor1_Speed(uint32_t speed){
	PWM_Write_Motor1(100-speed);
}
void Change_Motor2_Speed(uint32_t speed){
	PWM_Write_Motor2(100-speed);
}

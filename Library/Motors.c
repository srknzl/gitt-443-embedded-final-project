#include "Motors.h"
#include "GPIO.h"
#include "PWM.h"

/* For Motor controller:
 IN1 = P5
 IN2 = P6
 EN1 = P28
*/
void Motor1_ChangeDirection(MotorDirection direction){
	if(direction == STOPMOVING){	
		GPIO_PIN_Write(PORT1,MASK_IN1,1);
		GPIO_PIN_Write(PORT1,MASK_IN2,1);
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
		GPIO_PIN_Write(PORT1,MASK_IN3,1);
		GPIO_PIN_Write(PORT1,MASK_IN4,1);
	} else if(direction == COUNTERCLOCKWISE){	
		GPIO_PIN_Write(PORT1,MASK_IN3,0);
		GPIO_PIN_Write(PORT1,MASK_IN4,1);
	}	else if(direction == CLOCKWISE){
		GPIO_PIN_Write(PORT1,MASK_IN3,1);
		GPIO_PIN_Write(PORT1,MASK_IN4,0);
	}
}

void Move_Forward(){
	// Todo add led functionality
	Motor1_ChangeDirection(CLOCKWISE);
}
void Move_Backward(){
	// Todo add led functionality
	Motor1_ChangeDirection(COUNTERCLOCKWISE);
}
void Turn_Left(){
	Motor1_ChangeDirection(CLOCKWISE);
	// Todo add led functionality
	// Todo add turn left code
}
void Turn_Right(){
	Motor1_ChangeDirection(COUNTERCLOCKWISE);
	// Todo add led functionality
	// Todo add turn left code
}
void Stop_Motors(){
	// Todo add led functionality
	Motor1_ChangeDirection(STOPMOVING);
}
void Change_Speed(uint32_t speed){
	PWM_Write(100-speed);
}

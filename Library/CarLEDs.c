#include "CarLEDs.h"
#include "GPIO.h"
#include "DataStructures.h"

void CarLEDs_stop(){
	status.LED1_Status = OFF;
	status.LED2_Status = OFF;
	status.LED3_Status = OFF;
	status.LED4_Status = OFF;
}
void CarLEDs_turningLeft(){
	status.LED1_Status = OFF;
	status.LED2_Status = OFF;
	status.LED3_Status = BLINK_ON;
	status.LED4_Status = BLINK_ON;
}
void CarLEDs_turningRight(){
	status.LED1_Status = BLINK_ON;
	status.LED2_Status = BLINK_ON;
	status.LED3_Status = OFF;
	status.LED4_Status = OFF;
}
void CarLEDs_goingForward(){
	status.LED1_Status = OFF;
	status.LED2_Status = ON;
	status.LED3_Status = ON;
	status.LED4_Status = OFF;
}
void CarLEDs_goingBackward(){
	status.LED1_Status = ON;
	status.LED2_Status = OFF;
	status.LED3_Status = OFF;
	status.LED4_Status = ON;
}

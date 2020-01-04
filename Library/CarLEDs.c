#include "CarLEDs.h"
#include "GPIO.h"
#include "DataStructures.h"

/*
Desc: Change leds to be in stop mode
*/
void CarLEDs_stop(){
	status.LED1_Status = OFF;
	status.LED2_Status = OFF;
	status.LED3_Status = OFF;
	status.LED4_Status = OFF;
}
/*
Desc: Change leds to be in turn left mode
*/
void CarLEDs_turningLeft(){
	status.LED1_Status = OFF;
	status.LED2_Status = OFF;
	// if check needed for autonomous mode, continously calling this function causes leds to always be on 
	if(status.LED3_Status!= BLINK_ON && status.LED3_Status!= BLINK_OFF)status.LED3_Status = BLINK_ON;
	if(status.LED4_Status!= BLINK_ON && status.LED4_Status!= BLINK_OFF)status.LED4_Status = BLINK_ON;
}
/*
Desc: Change leds to be in turn right mode
*/
void CarLEDs_turningRight(){
	if(status.LED1_Status!= BLINK_ON && status.LED1_Status!= BLINK_OFF)status.LED1_Status = BLINK_ON;
	if(status.LED2_Status!= BLINK_ON && status.LED2_Status!= BLINK_OFF)status.LED2_Status = BLINK_ON;
	status.LED3_Status = OFF;
	status.LED4_Status = OFF;
}
/*
Desc: Change leds to be in move forward mode
*/
void CarLEDs_goingForward(){
	status.LED1_Status = OFF;
	status.LED2_Status = ON;
	status.LED3_Status = ON;
	status.LED4_Status = OFF;
}
/*
Desc: Change leds to be in move back mode
*/
void CarLEDs_goingBackward(){
	status.LED1_Status = ON;
	status.LED2_Status = OFF;
	status.LED3_Status = OFF;
	status.LED4_Status = ON;
}

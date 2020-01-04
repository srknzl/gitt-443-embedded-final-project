#include "LPC407x_8x_177x_8x.h"

#include <stdio.h> 
#include <string.h> 
#include <math.h>

#include "Library/Serial.h"
#include "Library/HM10.h"
#include "Library/GPIO.h"
#include "Library/Timer.h"
#include "Library/Ultrasonic.h"
#include "Library/ADC.h"
#include "Library/PWM.h"
#include "Library/SysTick.h"
#include "Library/Motors.h"
#include "Library/DataStructures.h"
#include "Library/CarLEDs.h"

char serialReceived[256]; // Data received from UART(by Putty) put here
//char receivedBlue[256]; 
Operations lastOperation; // Last operation that the car was doing before becoming underlight 
DeviceStatus status; // The global status object of the car
static uint32_t last_dist = 0; // Last distance that is measured before the current one

/*
Desc: This function is used to test ultrasonic sensor using LEDS, like in the Lab.
*/
void testUltrasonicWithLeds(){
	if(status.distance < 7){
		LED1_On();
		LED2_On();
		LED3_On();
		LED4_On();
	}else if(status.distance < 12){
		LED1_On();
		LED2_On();
		LED3_On();
		LED4_Off();
	}else if(status.distance < 20){
		LED1_On();
		LED2_On();
		LED3_Off();
		LED4_Off();
	}else if(status.distance < 30){
		LED1_On();
		LED2_Off();
		LED3_Off();
		LED4_Off();
	}else{
		LED1_Off();
		LED2_Off();
		LED3_Off();
		LED4_Off();
	}
}


void init() {
	PWM_Init();
	Serial_Init();
	HM10_Init();
	///*
	Ultrasonic_Init();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	Ultrasonic_Start_Trigger_Timer();
	//*/
	///* 
	LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
	//*/
	ADC_Init();
	
	// Initialize status object
	status.distance = 0;
	status.lightLevelLeft = 0;
	status.lightLevelRight = 0;
	status.opmode = "TEST";
	status.underLight = 0;
	status.currentOperation = STOP; // At the beginning car should stop 
	status.willContinue = 0;
	status.wheelToothCount = 0;
	CarLEDs_stop(); // Adjust leds according to stop operation
	
	
	Change_Motor1_Speed(100); // These do not move car just adjusts speed scaling to 100, meaning, potentiometer is not scaled in test mode
	Change_Motor2_Speed(100);
	
	HM10_SendCommand("TESTING"); // Send TESTING at the beginning since TEST mode is entered at first. 
	HM10_SendCRLN(); // Sends \r\n
	
	//Initialize GPIO pins
	GPIO_init();
	SysTick_Init();
}

void update() {
	uint32_t temp, temp_speed;
	if(serialNewDataAvailable){ // If the car got new data via UART(e.g putty)
		serialNewDataAvailable = 0;
		if(Serial_ResponseReceived()){ // if last received character is \r
			Serial_SendCRLN();
			Serial_SendString(serialReceived);
			memset(serialReceived, 0, 256); // empty serialReceived buffer for avoiding accumulation
		}else{
			strncat(serialReceived, &serialReceivedCharacter, 1); // If not append to form complete response
		}
	}
	
	if(HM10NewDataAvailable){ // If the car got new data via bluetooth
		HM10NewDataAvailable = 0;
		if(HM10_ResponseReceived()){  // if last received character is \n, the command is finished, start processing it
			//HM10_SendResponseToUart();
			HM10_ProcessResponse();
			HM10_ClearBuffer();
		}
	}
	
	if(ultrasonicSensorNewDataAvailable){ // If ultrasonic data is avaliable
		ultrasonicSensorNewDataAvailable = 0;
		status.distance = (ultrasonicSensorFallingCaptureTime - ultrasonicSensorRisingCaptureTime)/58;
		//testUltrasonicWithLeds();
	}
	
	if(ADC_New_Speed_Available){ // If new speed from potentiometer is available
		temp = (ADC_Get_Last_Speed()*100) / 4095;
		if (temp< 5){ // To reduce noise and problem we faced crop 0-5 interval to 0.
			status.speed = 0;
		}	else {
			status.speed = temp; // otherwise write the same 
		}
	}
	
	if(ADC_New_Left_Light_Available){ // If new left light level is available
		status.lightLevelLeft = 1023 - (ADC_Get_Last_Left_Light() / 4);
	}
	
	if(ADC_New_Right_Light_Available){ // If new right light level is available
		status.lightLevelRight = 1023 - (ADC_Get_Last_Right_Light() / 4);
	}
	// If light level is high on either side, set the variable 
	if(status.lightLevelLeft > LIGHT_THRESHOLD || status.lightLevelRight > LIGHT_THRESHOLD){ 
		status.underLight = 1;
	}else{
		status.underLight = 0;
	}
	
	// The code runs in test mode 
	if(strcmp(status.opmode,"TEST") == 0){
		
		Change_Motor1_Speed(100); // Do not apply scaling to potentiometer value 
		Change_Motor2_Speed(100);
		
		// Stop if moving forward or backward, due to 90 degree turn 
		if((status.currentOperation == FORWARD || status.currentOperation == BACKWARD) && status.underLight){ // Stop when light is on you.
			lastOperation = status.currentOperation;
			status.currentOperation = STOP;
			Stop_Motors();
			status.willContinue = 1;
		}
		// If the car is stopping, and stopped due to light, continue last operation 
		if(status.currentOperation == STOP && status.willContinue && status.underLight == 0){ 
			status.currentOperation = lastOperation;
			// according to last operation change leds and motor directions 
			if(status.currentOperation == FORWARD){ 
				CarLEDs_goingForward();
				Move_Forward();
			}else if(status.currentOperation == BACKWARD){
				CarLEDs_goingBackward();
				Move_Backward();
			}else if(status.currentOperation == STOP){
				CarLEDs_stop();
				Stop_Motors();
			}
			status.willContinue = 0; // willContinue should be 1 in stop mode and due to light is stop cause
		}
		
		// In the beginning turning left or right wheelToothCount is reset, stop if we reach the threshold for 90 degree turn
		if((status.currentOperation == LEFT || status.currentOperation == RIGHT) && status.wheelToothCount > TURN_COUNT_FOR_90){
			status.wheelToothCount = 0;
			status.currentOperation = STOP;
			Stop_Motors();
			CarLEDs_stop();
		}

		
	}else if(strcmp(status.opmode, "AUTO")==0){// The code runs in auto mode 
		
		if(status.started == 1){ // Only move if started
			// Clip distance to 40 maximum to avoid our speed functions' overflow situation
			status.distance = status.distance < 40 ? status.distance : 40; 
			if(status.underLight == 1){ // If underlight stop and respond FINISH
				status.started = 0;
				HM10_SendCommand("FINISH");
				HM10_SendCRLN();
			} 
			else if (status.distance < 10) { // If too close turn right
				Change_Motor2_Speed(10);
				Change_Motor1_Speed(70);
				CarLEDs_turningRight();
			} else { // Else depending on the distance adjust turn 
				temp_speed = (uint32_t) (8.0 + (75.0 / (1 + pow(2.718281828459, ((5.0 - status.distance) / 3.0)))));
				Change_Motor2_Speed(temp_speed);
				Change_Motor1_Speed(91 - temp_speed);
				if(temp_speed <  (91 - temp_speed)) {
					CarLEDs_turningRight();
				} else {
					CarLEDs_turningLeft();	
				}
			}
		} else if(status.started == 0) { // If not started stop 
			status.currentOperation = STOP;
			Stop_Motors();
			status.willContinue = 0;
			CarLEDs_stop();
		}
	}
}



int main() {
	init();
	
	while(1) {
		update();
	}
}


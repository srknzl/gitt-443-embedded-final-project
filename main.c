#include "LPC407x_8x_177x_8x.h"

#include <stdio.h> 
#include <string.h> 

#include "Library/Serial.h"
#include "Library/HM10.h"
#include "Library/GPIO.h"
#include "Library/Timer.h"
#include "Library/Ultrasonic.h"
#include "Library/ADC.h"

char serialReceived[256]; 
char receivedBlue[256];
DeviceStatus status;

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
	Serial_Init();
	HM10_Init();
	
	/*
	Ultrasonic_Init();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	Ultrasonic_Start_Trigger_Timer();
	//*/
	/* Needed for ultrasonic test
	LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
	//*/
	ADC_Init();
	ADC_Start();
	status.distance = 0;
	status.lightLevelLeft = 0;
	status.lightLevelRight = 0;
	status.opmode = "AUTO";
}

void update() {
	
	
	if(serialNewDataAvailable){
		serialNewDataAvailable = 0;
		if(Serial_ResponseReceived()){ // if last received character is \r
				Serial_SendCRLN();
				Serial_SendString(serialReceived);
				memset(serialReceived, 0, 256); // empty serialReceived buffer for avoiding accumulation
		}else{
			strncat(serialReceived, &serialReceivedCharacter, 1); // If not append to form complete response
		}
	}
	
	if(HM10NewDataAvailable){
		HM10NewDataAvailable = 0;
		if(HM10_ResponseReceived()){  // if last received character is \n
			HM10_SendResponseToUart();
			HM10_ProcessResponse(&status);
			HM10_ClearBuffer();
		}
	}
	/*
	if(ultrasonicSensorNewDataAvailable){
		ultrasonicSensorNewDataAvailable = 0;
		status.distance = (ultrasonicSensorFallingCaptureTime - ultrasonicSensorRisingCaptureTime)/58;
		testUltrasonicWithLeds();
	}
	*/
	if(ADC_New_Data_Available){
		status.lightLevelLeft = 1023 - (ADC_GetLastValue() / 4);
	}
}



int main() {
	init();
	
	while(1) {
		update();
	}
}


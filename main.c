#include "LPC407x_8x_177x_8x.h"

#include <stdio.h> 
#include <string.h> 

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

char serialReceived[256]; 
char receivedBlue[256];
Operations lastOperation;
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
	
	status.distance = 0;
	status.lightLevelLeft = 0;
	status.lightLevelRight = 0;
	status.opmode = "TEST";
	status.underLight = 0;
	status.currentOperation = STOP;
	status.willContinue = 0;
	status.turnCount = 0;
	
	Change_Motor1_Speed(0);
	Change_Motor2_Speed(0);
	
	HM10_SendCommand("TESTING");
	HM10_SendCRLN();
	
	//Initialize GPIO pins
	GPIO_init();
	SysTick_Init();
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
				//HM10_SendResponseToUart();
				HM10_ProcessResponse();
				HM10_ClearBuffer();
			}
		}
		
		if(ultrasonicSensorNewDataAvailable){
			ultrasonicSensorNewDataAvailable = 0;
			status.distance = (ultrasonicSensorFallingCaptureTime - ultrasonicSensorRisingCaptureTime)/58;
			//testUltrasonicWithLeds();
		}
		
		if(ADC_New_Speed_Available){
			status.speed = (ADC_Get_Last_Speed()*100) / 4095;
			Change_Motor1_Speed(status.speed);
			Change_Motor2_Speed(status.speed);
		}
		
		if(ADC_New_Left_Light_Available){
			status.lightLevelLeft = 1023 - (ADC_Get_Last_Left_Light() / 4);
		}
		
		if(ADC_New_Right_Light_Available){
			status.lightLevelRight = 1023 - (ADC_Get_Last_Right_Light() / 4);
		}
		
		if(status.lightLevelLeft > 500 || status.lightLevelRight > 500){
				status.underLight = 1;
		}else{
				status.underLight = 0;
		}
		
		
		if(strcmp(status.opmode,"TEST") == 0){
			if((status.currentOperation == FORWARD || status.currentOperation == BACKWARD) && status.underLight){ // Stop when light is on you.
				lastOperation = status.currentOperation;
				status.currentOperation = STOP;
				Stop_Motors();
				status.willContinue = 1;
			}
			
			if(status.currentOperation == STOP && status.willContinue && status.underLight == 0){ // continue last operation
				status.currentOperation = lastOperation;
				if(status.currentOperation == FORWARD){
						Move_Forward();
				}else if(status.currentOperation == BACKWARD){
						Move_Backward();
				}else if(status.currentOperation == STOP){
						Stop_Motors();
				}
				status.willContinue = 0;
			}
			
			if((status.currentOperation == LEFT || status.currentOperation == RIGHT) && status.turnCount > 6){
				status.turnCount = 0;
				status.currentOperation = STOP;
				Stop_Motors();
			}

			
		}else if(strcmp(status.opmode, "AUTO")==0){
			if(status.started){
				// Todo autonomous code 
			}else{
				status.currentOperation = STOP;
				Stop_Motors();
				status.willContinue = 0;
			}
		}
}



int main() {
	init();
	
	while(1) {
		update();
	}
}


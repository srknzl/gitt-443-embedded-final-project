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

char serialReceived[256]; 
char receivedBlue[256];
Operations lastOperation;
DeviceStatus status;
static uint32_t last_dist = 0;

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
	status.wheelToothCount = 0;
	CarLEDs_stop();
	
	
	Change_Motor1_Speed(100);
	Change_Motor2_Speed(100);
	
	HM10_SendCommand("TESTING");
	HM10_SendCRLN();
	
	//Initialize GPIO pins
	GPIO_init();
	SysTick_Init();
}

void update() {
		uint32_t temp, temp_speed;
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
			temp = (ADC_Get_Last_Speed()*100) / 4095;
			if (temp< 5){
					status.speed = 0;
			}	else {
				  status.speed = temp;
			}
		}
		
		if(ADC_New_Left_Light_Available){
			status.lightLevelLeft = 1023 - (ADC_Get_Last_Left_Light() / 4);
		}
		
		if(ADC_New_Right_Light_Available){
			status.lightLevelRight = 1023 - (ADC_Get_Last_Right_Light() / 4);
		}
		
		if(status.lightLevelLeft > LIGHT_THRESHOLD || status.lightLevelRight > LIGHT_THRESHOLD){
				status.underLight = 1;
		}else{
				status.underLight = 0;
		}
		
		
		if(strcmp(status.opmode,"TEST") == 0){
			
			Change_Motor1_Speed(100); // status.speed
			Change_Motor2_Speed(100);
			
			if((status.currentOperation == FORWARD || status.currentOperation == BACKWARD) && status.underLight){ // Stop when light is on you.
				lastOperation = status.currentOperation;
				status.currentOperation = STOP;
				Stop_Motors();
				status.willContinue = 1;
			}
			
			if(status.currentOperation == STOP && status.willContinue && status.underLight == 0){ // continue last operation
				status.currentOperation = lastOperation;
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
				status.willContinue = 0;
			}
			
			if((status.currentOperation == LEFT || status.currentOperation == RIGHT) && status.wheelToothCount > TURN_COUNT_FOR_90){
				status.wheelToothCount = 0;
				status.currentOperation = STOP;
				Stop_Motors();
				CarLEDs_stop();
			}

			
		}else if(strcmp(status.opmode, "AUTO")==0){
			
			if(status.started == 1){
					status.distance = status.distance < 40 ? status.distance : 40;
					if(status.underLight == 1){
						status.started = 0;
						HM10_SendCommand("FINISH");
						HM10_SendCRLN();
					} 
					/*
					else if (status.distance < 10) {
							Change_Motor2_Speed(0);
							Change_Motor1_Speed(20);
							CarLEDs_turningRight();
					} else if (status.distance < 30) {
							Change_Motor2_Speed(40);
							Change_Motor1_Speed(40);
							CarLEDs_goingForward();
					} else if (status.distance < 50) {
							Change_Motor2_Speed(20);
							Change_Motor1_Speed(0);
							CarLEDs_turningLeft();
					}
					*/
					/*
					if (status.distance < 15) {
							if (last_dist < status.distance) {
									Change_Motor2_Speed(75);
									Change_Motor1_Speed(7);
							} else if (last_dist == status.distance) {
									Change_Motor2_Speed(7);
									Change_Motor1_Speed(75);
							} else if (last_dist > status.distance) {
									Change_Motor2_Speed(0);
									Change_Motor1_Speed(85);
							}
					} else if (status.distance < 30) {
							Change_Motor2_Speed(40);
							Change_Motor1_Speed(40);
					} else if (status.distance >= 30) {
							if (last_dist < status.distance) {
									Change_Motor2_Speed(3);
									Change_Motor1_Speed(85);
							} else if (last_dist == status.distance) {
									Change_Motor2_Speed(75);
									Change_Motor1_Speed(3);
							} else if (last_dist > status.distance) {
									Change_Motor2_Speed(40);
									Change_Motor1_Speed(40);	
							}
					} 
					last_dist = status.distance;
					*/
				
					
					else if (status.distance < 10) {
						Change_Motor2_Speed(10);
						Change_Motor1_Speed(70);
						CarLEDs_turningRight();
					} else {
						temp_speed = (uint32_t) (8.0 + (75.0 / (1 + pow(2.718281828459, ((5.0 - status.distance) / 3.0)))));
						Change_Motor2_Speed(temp_speed);
						Change_Motor1_Speed(91 - temp_speed);
						if(temp_speed <  (91 - temp_speed)) {
							CarLEDs_turningRight();
						} else {
							CarLEDs_turningLeft();	
						}
					}
					
					
					//Change_Motor2_Speed(10 + (2 * status.distance));
					//Change_Motor1_Speed(90 - (2 * status.distance));
			}	else if(status.started == 0) {
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


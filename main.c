#include "LPC407x_8x_177x_8x.h"

#include <stdio.h> 
#include <string.h> 

#include "Library/Serial.h"
#include "Library/HM10.h"

char serialReceived[256]; 
char receivedBlue[256];
DeviceStatus status;


void init() {	
	Serial_Init();
	HM10_Init();
	status.distance = 10;
	status.lightLevelLeft = 11;
	status.lightLevelRight = 122;
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
}

int main() {
	init();
	
	while(1) {
		update();
	}
}


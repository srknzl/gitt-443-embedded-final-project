#include "LPC407x_8x_177x_8x.h"

#include <stdio.h> 
#include <string.h> 

#include "Library/Serial.h"
#include "Library/HM10.h"

char received[100]; 
char receivedBlue[256];

void init() {	
	Serial_Init();
	//serialTransmitData = "Selam bro";
	//Serial_SendData();
	
	HM10_Init();
	//HM10_SendCommand("AT\r\n");
}

void update() {
	/*
	while(*serialTransmitData){
			Serial_WriteData(*serialTransmitData++);
			while(!serialTransmitCompleted);
	}
	// */
	if(serialNewDataAvailable){
		serialNewDataAvailable = 0;
		if(serialReceivedCharacter == '\r'){
			serialTransmitData = received;
			strncat(received, "\r\n", 2);
			HM10_SendCommand(received);  // Upon Received a enter send the word to hm10 
			//Serial_SendData();
			memset(received, 0, 100); // empty received
		}else{
			strncat(received, &serialReceivedCharacter, 1);
		}
	}
	if(HM10NewDataAvailable){
		HM10NewDataAvailable = 0; 
		if(HM10Buffer[HM10CurrentBufferIndex-1] == '\n'){ // If last character was \n the response finished
			strcpy(receivedBlue, HM10Buffer); // Copy the response 
			HM10_ClearBuffer(); 
			serialTransmitData = receivedBlue; // set response to be transmitted to putty via uart
			Serial_SendData();
			memset(received, 0, 256); // empty received
		}
	}
}

int main() {
	init();
	
	while(1) {
		update();
	}
}


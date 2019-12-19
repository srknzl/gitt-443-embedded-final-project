#include "LPC407x_8x_177x_8x.h"

#include <stdio.h> 
#include <string.h> 

#include "Library/Serial.h"
#include "Library/HM10.h"

char received[100]; 

void init() {	
	Serial_Init();
	//serialTransmitData = "Selam bro";
	//Serial_SendData();
	
	HM10_Init();
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
			strncat(serialTransmitData, "\r\n", 2);
			Serial_SendData();
			strcpy(received ,"");
		}else{
			strncat(received, &serialReceivedCharacter, 1);
		}
	}
}

int main() {
	init();
	
	while(1) {
		update();
	}
}


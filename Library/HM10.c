#include "HM10.h"
#include "math.h"
#include "Motors.h"
#include "Serial.h"
#include "DataStructures.h"
#include "SystemStructures.h"
#include "CarLEDs.h"

#include <stdio.h>
#include <string.h>

uint8_t HM10CurrentBufferIndex = 0; // The last char index that is inserted
uint8_t HM10NewDataAvailable = 0; // There is a new char added
char statusString[120]; // Status string to be sent

char HM10Buffer[HM10BufferSize]; // Buffer that is holding information received

void HM10_Init() {	
	HM10_UART_TX_PIN |= 0x02; // Set func to U3_TXD IOCON
	HM10_UART_RX_PIN |= 0x02; // Set func to U3_RXD IOCON
	
	PCONP |= 1 <<  25; // Turn on UART3
	
	HM10_UART->FCR =	1 << 0 // Enable fifos
						 |	0 << 1
						 |	0 << 2
						 |	0 << 6;
	
	HM10_UART->LCR |= (1 << 7); // Enable access to divisor latches
	
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	HM10_UART->DLM = 0x01;
	HM10_UART->DLL = 0x25;
	HM10_UART->FDR = 0x01 << 0 | 0x03 << 4;

	HM10_UART->LCR &= ~(1 << 7); // Disable divisor latch access
	
	HM10_UART->LCR =	3 << 0 // 8 bit char, 1 stop bit and odd parity config
							| 0 << 2
							| 0 << 3
							| 0 << 4;
	
	//Enable the Receive Data Available Interrupt.
	HM10_UART->IER |= 1;
	//Enable UART3_IRQn Interrupt.
	NVIC_EnableIRQ(UART3_IRQn);
	
}
/*
Desc: Sends a data to connected bluetooth device
*/
void HM10_SendCommand(char* command) {
	HM10_Write(command);
}

/*
Desc: Clears the HM10 Buffer
*/
void HM10_ClearBuffer() {
	uint32_t length;
	HM10CurrentBufferIndex = 0;
	length = strlen(HM10Buffer);
	if (length < HM10BufferSize){  // If length is less than buffer size memset may not write 0 to all the buffer.
			memset(HM10Buffer,0, length);
	}else{
			memset(HM10Buffer,0, HM10BufferSize);	// write 0 to all buffer entries.
	}
}
/*
Desc: Waits for a char to be received and reads the char 
*/
char HM10_ReadData() {
	while (!(HM10_UART->LSR & 0x01));
	return HM10_UART->RBR;
}
/*
Desc: Wait for the previous char to be sent, make a char to be sended char 
*/
void HM10_WriteData(char data) {
	while (!(HM10_UART->LSR & 0x20));
	HM10_UART->THR = data;
}
/*
Desc: Send chars one by one till the end of a string
*/
void HM10_Write(char* data) {
	while(*data > 0)  {
		HM10_WriteData(*data++);
	}
}
/*
Desc: Handles received response that is inside the buffer.
*/
void HM10_SendResponseToUart(){
		serialTransmitData = HM10Buffer; // Transmit the response to uart
		Serial_SendData(); 
}
/*
Desc: Responds incoming messages that is inside HM10Buffer. All commands are echoed back
*/
void HM10_ProcessResponse(){
		if(strcmp(HM10Buffer,"STATUS\r\n")==0){ // Send status string 
			HM10_SendCommand("STATUS"); 
			HM10_SendCRLN();
			HM10_SendCommand(getStatusString());
			HM10_SendCRLN();
		}else if(strcmp(HM10Buffer,"LEFT\r\n")==0){ // Turn Left if in TEST mode
			HM10_SendCommand("LEFT");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = LEFT;
				Turn_Left();
				CarLEDs_turningLeft();
			}
		}else if(strcmp(HM10Buffer,"RIGHT\r\n")==0){ // Turn Right if in TEST mode
			HM10_SendCommand("RIGHT");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = RIGHT;
				Turn_Right();
				CarLEDs_turningRight();
			}
		}else if(strcmp(HM10Buffer,"FORWARD\r\n")==0){ // Move forward if in TEST mode
			HM10_SendCommand("FORWARD");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = FORWARD;
				Move_Forward();
				CarLEDs_goingForward();
			}
		}else if(strcmp(HM10Buffer,"BACK\r\n")==0){ // Move back if in TEST mode
			HM10_SendCommand("BACK");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = BACKWARD;
				Move_Backward();
				CarLEDs_goingBackward();
			}
		}else if(strcmp(HM10Buffer,"STOP\r\n")==0){ // Stop if in TEST mode
			HM10_SendCommand("STOP");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = STOP;
				Stop_Motors();
				status.willContinue = 0;
				CarLEDs_stop();
			}
		}else if(strcmp(HM10Buffer,"START\r\n")==0){ // Start if in AUTO mode
			HM10_SendCommand("START");
			HM10_SendCRLN();
			if(strcmp(status.opmode,"AUTO")==0){
				status.started = 1;
				Move_Forward();
			}
		}else if(strcmp(HM10Buffer,"AUTO\r\n")==0){  // Switch to AUTO if in TEST mode
			HM10_SendCommand("AUTO");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = STOP;
				Stop_Motors();
				status.willContinue = 0;
				CarLEDs_stop();
				HM10_SendCommand("AUTONOMOUS"); // Send additional AUTONOMOUS upon switch 
				HM10_SendCRLN();
				status.opmode = "AUTO";
			}
		}else if(strcmp(HM10Buffer,"TEST\r\n")==0){  // Switch to TEST if in AUTO mode
			HM10_SendCommand("TEST");
			HM10_SendCRLN();
			status.currentOperation = STOP;
			Stop_Motors();
			status.willContinue = 0;
			CarLEDs_stop();
			status.started =0;
			if(strcmp(status.opmode, "AUTO")==0){
				HM10_SendCommand("TESTING"); // Send additional TESTING upon switch 
				HM10_SendCRLN();
				status.opmode = "TEST";
			}
		}
}
/*
Desc: Sends \r\n to terminate a command
*/
void HM10_SendCRLN(){
	HM10_SendCommand("\r\n");
}

/*
Desc: Given status returns a json formatted string that represents the current status of the car
*/
char* getStatusString(){
	//{"distance":5,"light_level_left":150,"light_level_right":200,"op_mode":"AUTO"}
	snprintf(statusString, 120, "{\"distance\":%u,\"light_level_left\":%u,\"light_level_right\":%u,\"op_mode\":\"%s\"}", 
	status.distance,
	//(uint32_t) (5.0 + (70.0 / (1 + pow(2.718281828459, ((5.0 - status.distance) / 3.0))))),
	status.lightLevelLeft,
	status.lightLevelRight,
	status.opmode);
	return statusString;
}

/*
Desc: Returns 1 if last received char is \n otherwise return 0
*/
uint8_t HM10_ResponseReceived(){
	return HM10Buffer[HM10CurrentBufferIndex-1] == '\n';
}
/*
Desc: Upon getting a char, write it to HM10Buffer increase index by 1 and make HM10NewDataAvailable 1.
*/
void UART3_IRQHandler() {
	char data;
	data = HM10_ReadData();
	HM10Buffer[HM10CurrentBufferIndex] = data;
	HM10CurrentBufferIndex++;
	HM10NewDataAvailable = 1;
}



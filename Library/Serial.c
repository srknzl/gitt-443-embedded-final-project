#include "Serial.h"
#include "string.h"
#include "HM10.h"

char serialReceivedCharacter = 0;
uint8_t serialNewDataAvailable = 0;
char* serialTransmitData = 0;
uint8_t serialTransmitCompleted = 0;


void Serial_Init() {
	uint32_t temp;
	//Change the function of TX and RX pins for UART.
	temp = Serial_UART_TX_PIN;
	temp |= 1;
	temp &= ~(1<<1 | 1<<2);
	Serial_UART_TX_PIN = temp;
	
	temp = Serial_UART_RX_PIN;
	temp |= 1;
	temp &= ~(1<<1 | 1<<2);
	Serial_UART_RX_PIN = temp;
	//Turn on UART0.
	PCONP |= 1 << 3;
	//Enable FIFO for UART0.
	LPC_UART0->FCR |= 1;
	//In order to change the DLM, DLL and FDR values, Write correct code for enabling the access to Divisor Latches.
	//DLM = 0x00; DLL = 0x13; FDR = 0x05 << 0 | 0x07 << 4; 
	LPC_UART0->LCR |= 1 << 7;
	//Write correct DLM, DLL and FDR values for 115200 baudrate
	LPC_UART0->DLM = 0x00;
	LPC_UART0->DLL = 0x13;
	LPC_UART0->FDR = 0x05 << 0 | 0x07 << 4; 
	//Write correct code for disabling the access to Divisor Latches.
	LPC_UART0->LCR &= ~(1 << 7);
	//Change LCR register value for 8-bit character transfer, 1 stop bits and Even Parity.
	//011011
	temp = LPC_UART0->LCR;
	temp |= (1 | 1<<1 | 1<<3 | 1<<4);
	temp &= ~(1<<2 | 1<<5);
	LPC_UART0->LCR = temp;
	//Enable the Receive Data Available and THRE Interrupt.
	LPC_UART0->IER |= (1 | 1<<1);
	//Enable UART0_IRQn Interrupt.
	NVIC_EnableIRQ(UART0_IRQn);
	//Set UART0_IRQn Priority to 5.
	NVIC_SetPriority(UART0_IRQn, 5);
}

void UART0_IRQHandler() {	
	
	uint32_t currentInterrupt = ((LPC_UART0->IIR >> 1) & 7);
	
	//First if statement is for Receive Data Available interrupt.
	//When Receive Data Available, the code will enter to the first if condition.
	if(currentInterrupt == 0x02) {
		serialReceivedCharacter = Serial_ReadData();
		serialNewDataAvailable = 1;
	}
	//Second if statement is for THRE interrupt
	//When THRE interrupt is handled, the code will enter to the second if condition.
	else if(currentInterrupt == 0x01) {
		if(*serialTransmitData > 0) {
			Serial_WriteData(*serialTransmitData++);
		}
		else {
			serialTransmitCompleted = 1;
		}
	}
}

char Serial_ReadData() {
	return Serial_UART->RBR;
}

void Serial_WriteData(const char data) {
	serialTransmitCompleted = 0;
	Serial_UART->THR = data;
}

void Serial_SendData(){
	while(*serialTransmitData){
		Serial_WriteData(*serialTransmitData++);
		while(!serialTransmitCompleted);
	}
}
/*
Indicates if response character sequence is ended. Last character is always \r in putty.
which is sent when enter key is pressed.
*/
uint8_t Serial_ResponseReceived(){
	return serialReceivedCharacter == '\r';
}

void Serial_SendCRLN(){
	serialTransmitData = "\r\n";
	Serial_SendData();
}

void Serial_SendString(char * data){
	serialTransmitData = data;
	Serial_SendData();
}

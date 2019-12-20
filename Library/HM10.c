#include "HM10.h"
#include "math.h"

uint8_t HM10CurrentBufferIndex = 0;
uint8_t HM10NewDataAvailable = 0;

char HM10Buffer[HM10BufferSize];

void HM10_Init() {	
	HM10_UART_TX_PIN |= 0x02;
	HM10_UART_RX_PIN |= 0x02;
	
	PCONP |= 1 <<  25;
	
	HM10_UART->FCR =	1 << 0
						 |	0 << 1
						 |	0 << 2
						 |	0 << 6;
	
	HM10_UART->LCR |= (1 << 7);
	
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	HM10_UART->DLM = 0x01;
	HM10_UART->DLL = 0x25;
	HM10_UART->FDR = 0x01 << 0 | 0x03 << 4;

	HM10_UART->LCR &= ~(1 << 7);
	
	HM10_UART->LCR =	3 << 0
							| 0 << 2
							| 0 << 3
							| 0 << 4;
	
	//Enable the Receive Data Available Interrupt.
	HM10_UART->IER |= 1;
	//Enable UART3_IRQn Interrupt.
	NVIC_EnableIRQ(UART3_IRQn);
	
}

void HM10_SendCommand(char* command) {
	HM10_Write(command);
}

void HM10_ClearBuffer() {
	uint32_t length;
	HM10CurrentBufferIndex = 0;
	length = strlen(HM10Buffer);
	if (length < HM10BufferSize){  // If length is less than buffer size memset may not write 0 to all the buffer.
			memset(HM10Buffer,0, length);
	}else{
			memset(HM10Buffer,0, HM10BufferSize);	
	}
}

char HM10_ReadData() {
	while (!(HM10_UART->LSR & 0x01));
	return HM10_UART->RBR;
}

void HM10_WriteData(char data) {
	while (!(HM10_UART->LSR & 0x20));
	HM10_UART->THR = data;
}

void HM10_Write(char* data) {
	while(*data > 0)  {
		HM10_WriteData(*data++);
	}
}

// Handles received response that is inside the buffer.
void HM10_SendResponseToUart(){
		serialTransmitData = HM10Buffer; // Transmit the response to uart
		Serial_SendData();
		HM10_ClearBuffer(); 
}
// Communication protocol.
void HM10_ProcessResponse(){
		
}
uint8_t HM10_ResponseReceived(){
	return HM10Buffer[HM10CurrentBufferIndex-1] == '\n';
}

void UART3_IRQHandler() {
	char data;
	data = HM10_ReadData();
	HM10Buffer[HM10CurrentBufferIndex] = data;
	HM10CurrentBufferIndex++;
	HM10NewDataAvailable = 1;
}



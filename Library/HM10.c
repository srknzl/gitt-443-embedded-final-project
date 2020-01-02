#include "HM10.h"
#include "math.h"
#include "PWM.h"

uint8_t HM10CurrentBufferIndex = 0;
uint8_t HM10NewDataAvailable = 0;
char statusString[120];

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
}
// Communication protocol.
void HM10_ProcessResponse(){
		if(strcmp(HM10Buffer,"STATUS\r\n")==0){
			HM10_SendCommand("STATUS");
			HM10_SendCRLN();
			HM10_SendCommand(getStatusString());
			HM10_SendCRLN();
		}else if(strcmp(HM10Buffer,"LEFT\r\n")==0){
			HM10_SendCommand("LEFT");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = LEFT;
				Turn_Left();
			}
		}else if(strcmp(HM10Buffer,"RIGHT\r\n")==0){
			HM10_SendCommand("RIGHT");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = RIGHT;
				Turn_Right();
			}
		}else if(strcmp(HM10Buffer,"FORWARD\r\n")==0){
			HM10_SendCommand("FORWARD");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = FORWARD;
				Move_Forward();
			}
		}else if(strcmp(HM10Buffer,"BACK\r\n")==0){
			HM10_SendCommand("BACK");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = BACKWARD;
				Move_Backward();
			}
		}else if(strcmp(HM10Buffer,"STOP\r\n")==0){
			HM10_SendCommand("STOP");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				status.currentOperation = STOP;
				Stop_Motors();
				status.willContinue = 0;
			}
		}else if(strcmp(HM10Buffer,"START\r\n")==0){
			HM10_SendCommand("START");
			HM10_SendCRLN();
			if(strcmp(status.opmode,"AUTO")==0){
				status.started = 1;
			}
		}else if(strcmp(HM10Buffer,"AUTO\r\n")==0){
			HM10_SendCommand("AUTO");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "TEST")==0){
				HM10_SendCommand("AUTONOMOUS");
				HM10_SendCRLN();
				status.opmode = "AUTO";
			}
		}else if(strcmp(HM10Buffer,"TEST\r\n")==0){
			HM10_SendCommand("TEST");
			HM10_SendCRLN();
			if(strcmp(status.opmode, "AUTO")==0){
				HM10_SendCommand("TESTING");
				HM10_SendCRLN();
				status.opmode = "TEST";
			}
		}
}
void HM10_SendCRLN(){
	HM10_SendCommand("\r\n");
}

/*
Given status returns a json formatted string that represents the current status of the car

*/
char* getStatusString(){
	//{"distance":5,"light_level_left":150,"light_level_right":200,"op_mode":"AUTO"}
	snprintf(statusString, 120, "{\"distance\":%u,\"light_level_left\":%u,\"light_level_right\":%u,\"op_mode\":\"%s\"}", 
	status.distance,
	status.lightLevelLeft,
	status.lightLevelRight,
	status.opmode);
	return statusString;
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



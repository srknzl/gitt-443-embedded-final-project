#ifndef HM10_H
#define HM10_H

#include "LPC407x_8x_177x_8x.h"

#include "Serial.h"

#include <stdio.h>
#include <string.h>

#define HM10BufferSize 256

#define HM10_UART_BASE	0x4009C000
#define HM10_UART	((UART_TypeDef*) HM10_UART_BASE)

#define HM10_UART_TX_PIN_ADDRESS	0x4002C000
#define HM10_UART_TX_PIN	*((volatile uint32_t*)(HM10_UART_TX_PIN_ADDRESS))

#define HM10_UART_RX_PIN_ADDRESS	0x4002C004
#define HM10_UART_RX_PIN	*((volatile uint32_t*)(HM10_UART_RX_PIN_ADDRESS))

extern uint8_t HM10CurrentBufferIndex;
extern uint8_t HM10NewDataAvailable;

extern char HM10Buffer[HM10BufferSize];

void HM10_Init(void);
void HM10_SendCommand(char* command);
void HM10_ClearBuffer(void);

char HM10_ReadData(void);
void HM10_WriteData(char data);
void HM10_Write(char* data);


#endif

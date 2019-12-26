#include "GPIO.h"

void GPIO_DIR_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->DIR &= ~MASK;
	}
	else {
		PORT->DIR |= MASK;
	}
}

void GPIO_PIN_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->PIN &= ~MASK;
	}
	else {
		PORT->PIN |= MASK;
	}
}

void LED1_Init() {
	//Write code for initializing LED1.
	GPIO_DIR_Write(LED1_PORT,LED1_MASK,OUTPUT);
}

void LED2_Init() {
	//Write code for initializing LED2.
	GPIO_DIR_Write(LED2_PORT,LED2_MASK,OUTPUT);
}

void LED3_Init() {
	//Write code for initializing LED3.
	GPIO_DIR_Write(LED3_PORT,LED3_MASK,OUTPUT);
}

void LED4_Init() {
	//Write code for initializing LED4.
	GPIO_DIR_Write(LED4_PORT,LED4_MASK,OUTPUT);
}

void LED1_Off() {
	//Write code for turning off LED1.
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,HIGH);
}

void LED2_Off() {
	//Write code for turning off LED2.
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,HIGH);
}

void LED3_Off() {
	//Write code for turning off LED3.
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,LOW);
}

void LED4_Off() {
	//Write code for turning off LED4.
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,LOW);
}

void LED1_On() {
	//Write code for turning on LED1.
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,LOW);
}

void LED2_On() {
	//Write code for turning on LED2.
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,LOW);
}

void LED3_On() {
	//Write code for turning on LED3.
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,HIGH);
}

void LED4_On() {
	//Write code for turning on LED4.
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,HIGH);
}


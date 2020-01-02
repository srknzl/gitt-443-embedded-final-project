#include "GPIO.h"
#include "SystemStructures.h"
#include "DataStructures.h"


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
	GPIO_DIR_Write(LED3_PORT,LED3_MASK,OUTPUT);
}

void LED4_Init() {
	GPIO_DIR_Write(LED4_PORT,LED4_MASK,OUTPUT);
}

void LED1_Off() {
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,LOW);
}

void LED2_Off() {
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,LOW);
}

void LED3_Off() {
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,LOW);
}

void LED4_Off() {
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,LOW);
}

void LED1_On() {
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,HIGH);
}

void LED2_On() {
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,HIGH);
}

void LED3_On() {
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,HIGH);
}

void LED4_On() {
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,HIGH);
}

void LED1_Blink() {
	status.LED1_Status = BLINK_ON;
}

void LED2_Blink() {
	status.LED2_Status = BLINK_ON;
}

void LED3_Blink() {
	status.LED3_Status = BLINK_ON;
}

void LED4_Blink() {
	status.LED4_Status = BLINK_ON;
}

void GPIO_init(){

	IOCON_P1_23 &= ~7;
	IOCON_P1_24 &= ~7;
	IOCON_P0_21 &= ~7;
	
	status.LED1_Status = OFF; // Sag arka ON da çalisiyor
	status.LED2_Status = OFF; // Sag on ON da çalisiyor 
	status.LED3_Status = OFF; // Sol on ON da calisiyor
	status.LED4_Status = OFF; // Sol arka ON da çalisiyor 

	GPIO_DIR_Write(PORT1,MASK_IN1,1); // Make motor 1 control pins output
	GPIO_DIR_Write(PORT1,MASK_IN2,1);
	GPIO_DIR_Write(PORT5,MASK_IN3,1); // Make motor 2 control pins output
	GPIO_DIR_Write(PORT5,MASK_IN4,1);
	
	GPIO_ENF_PORT0 |= 1 << 21;
	NVIC_EnableIRQ(GPIO_IRQn);
	
	GPIO_DIR_Write(PORT1,MASK_SPEED,0); // Make speed sensor pin input
		
	NVIC_EnableIRQ(GPIO_IRQn);
		
	
	PORT1->PIN|=  (1 << 24) | (1 << 23); //initialize the motor
}

// TODO: check variable names
void update_LEDs() {
	if(status.LED1_Status == OFF) {
		LED1_Off();
	} else if(status.LED1_Status == ON) {	
		LED1_On();
	} else if(status.LED1_Status == BLINK_ON) {	
		LED1_On();
		status.LED1_Status = BLINK_OFF;
	} else if(status.LED1_Status == BLINK_OFF) {	
		LED1_Off();
		status.LED1_Status = BLINK_ON;
	}
	if(status.LED2_Status == OFF) {	
		LED2_Off();
	} else if(status.LED2_Status == ON) {	
		LED2_On();
	} else if(status.LED2_Status == BLINK_ON) {	
		LED2_On();
		status.LED2_Status = BLINK_OFF	;
	} else if(status.LED2_Status == BLINK_OFF) {	
		LED2_Off();
		status.LED2_Status = BLINK_ON;
	}
	if(status.LED3_Status == OFF) {			
		LED3_Off();
	} else if(status.LED3_Status == ON) {
		LED3_On();
	} else if(status.LED3_Status == BLINK_ON) {	
		LED3_On();
		status.LED3_Status = BLINK_OFF;
	} else if(status.LED3_Status == BLINK_OFF) {	
		LED3_Off();
		status.LED3_Status = BLINK_ON;
	}
	if(status.LED4_Status == OFF) {				
		LED4_Off();
	} else if(status.LED4_Status == ON) {	
		LED4_On();
	} else if(status.LED4_Status == BLINK_ON) {	
		LED4_On();
		status.LED4_Status = BLINK_OFF;
	} else if(status.LED4_Status == BLINK_OFF) {	
		LED4_Off();
		status.LED4_Status = BLINK_ON;
	}
}

void GPIO_IRQHandler(){
	if((GPIO_FALLING_INTERRUPT_STATUS_PORT0 & SPEEDSENSOR_MASK) > 0){
		GPIO_CLEAR_INTERRUPT_PORT0 |= SPEEDSENSOR_MASK;
		if(status.currentOperation == LEFT || status.currentOperation == RIGHT){
			status.wheelToothCount++;
		}
		
	}
}

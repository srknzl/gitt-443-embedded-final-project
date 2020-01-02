#include "GPIO.h"
#include "SystemStructures.h"
#include "DataStructures.h"

static LED_STATUS LED1_Status = OFF;
static LED_STATUS LED2_Status = ON;
static LED_STATUS LED3_Status = BLINK_ON;
static LED_STATUS LED4_Status = BLINK_OFF;

static uint32_t wheelToothCount = 0;

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
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,HIGH);
}

void LED2_Off() {
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,HIGH);
}

void LED3_Off() {
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,LOW);
}

void LED4_Off() {
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,LOW);
}

void LED1_On() {
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,LOW);
}

void LED2_On() {
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,LOW);
}

void LED3_On() {
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,HIGH);
}

void LED4_On() {
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,HIGH);
}

void LED1_Blink() {
	LED1_Status = BLINK_ON;
}

void LED2_Blink() {
	LED2_Status = BLINK_ON;
}

void LED3_Blink() {
	LED3_Status = BLINK_ON;
}

void LED4_Blink() {
	LED4_Status = BLINK_ON;
}

void GPIO_init(){

	IOCON_P1_23 &= ~7;
	IOCON_P1_24 &= ~7;
	IOCON_P0_21 &= ~7;
	
	GPIO_DIR_Write(PORT1,MASK_IN1,1); // Make motor control pins output
	GPIO_DIR_Write(PORT1,MASK_IN2,1);
	GPIO_DIR_Write(PORT5,MASK_IN3,1); // Make motor 2 control pins output
	GPIO_DIR_Write(PORT5,MASK_IN4,1);
	
	
	GPIO_DIR_Write(PORT1,MASK_SPEED,0); // Make speed sensor pin input
	
	GPIO_INTERRUPT->ENF0 |= 1 << 21; // Enable falling interrupt for speed sensor.
	
	NVIC_EnableIRQ(GPIO_IRQn);
		
	
	PORT1->PIN|=  (1 << 24) | (1 << 23); //initialize the motor
}

// TODO: check variable names
void update_LEDs() {
	if(LED1_Status == OFF) {
		LED1_Off();
	} else if(LED1_Status == ON) {	
		LED1_On();
	} else if(LED1_Status == BLINK_ON) {	
		LED1_On();
		LED1_Status = BLINK_OFF;
	} else if(LED1_Status == BLINK_OFF) {	
		LED1_Off();
		LED1_Status = BLINK_ON;
	}
	if(LED2_Status == OFF) {	
		LED2_Off();
	} else if(LED2_Status == ON) {	
		LED2_On();
	} else if(LED2_Status == BLINK_ON) {	
		LED2_On();
		LED2_Status = BLINK_OFF	;
	} else if(LED2_Status == BLINK_OFF) {	
		LED2_Off();
		LED2_Status = BLINK_ON;
	}
	if(LED3_Status == OFF) {			
		LED3_Off();
	} else if(LED3_Status == ON) {
		LED3_On();
	} else if(LED3_Status == BLINK_ON) {	
		LED3_On();
		LED3_Status = BLINK_OFF;
	} else if(LED3_Status == BLINK_OFF) {	
		LED3_Off();
		LED3_Status = BLINK_ON;
	}
	if(LED4_Status == OFF) {				
		LED4_Off();
	} else if(LED4_Status == ON) {	
		LED4_On();
	} else if(LED4_Status == BLINK_ON) {	
		LED4_On();
		LED4_Status = BLINK_OFF;
	} else if(LED4_Status == BLINK_OFF) {	
		LED4_Off();
		LED4_Status = BLINK_ON;
	}
}

void GPIO_IRQHandler(){
	if((GPIO_INTERRUPT->STATF0 & MASK_SPEED) > 0){ // If speed sensor's falling interrupt 
		GPIO_INTERRUPT->CLR0 |= MASK_SPEED; // Clear the interrput first
		wheelToothCount++; // Increase wheel tooth count
		if(wheelToothCount == 6){ // The wheel has 6 tooth
			status.turnCount++; // If tooth count is 6 increase turn count by 1
			wheelToothCount = 0; // Reset wheel tooth count 
		}
	}
}

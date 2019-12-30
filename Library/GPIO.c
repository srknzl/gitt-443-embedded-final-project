#include "GPIO.h"
#include "SystemStructures.h"

static uint32_t LED1_Status = 0;
static uint32_t LED2_Status = 1;
static uint32_t LED3_Status = 2;
static uint32_t LED4_Status = 3;

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

void LED1_Blink() {
	LED1_Status = 2;
}

void LED2_Blink() {
	LED2_Status = 2;
}

void LED3_Blink() {
	LED3_Status = 2;
}

void LED4_Blink() {
	LED4_Status = 2;
}

void GPIO_init(){
	IOCON_P1_23 &= ~7;
	IOCON_P1_24 &= ~7;
	GPIO_DIR_Write(PORT1,MC_IN1,1);
	GPIO_DIR_Write(PORT1,MC_IN2,1);
	
		
	//initializes the motor
	PORT1->PIN|=  (1 << 24) | (1 << 23);
}

// TODO: check variable names
void update_LEDs() {
	if(LED1_Status == 0) {				// off
		LED1_Off();
	} else if(LED1_Status == 1) {	// on
		LED1_On();
	} else if(LED1_Status == 2) {	// blinking, on
		LED1_On();
		LED1_Status = 3;
	} else if(LED1_Status == 3) {	// blinking, off
		LED1_Off();
		LED1_Status = 2;
	}
	if(LED2_Status == 0) {				// off
		LED2_Off();
	} else if(LED2_Status == 1) {	// on
		LED2_On();
	} else if(LED2_Status == 2) {	// blinking, on
		LED2_On();
		LED2_Status = 3;
	} else if(LED2_Status == 3) {	// blinking, off
		LED2_Off();
		LED2_Status = 2;
	}
	if(LED3_Status == 0) {				// off
		LED3_Off();
	} else if(LED3_Status == 1) {	// on
		LED3_On();
	} else if(LED3_Status == 2) {	// blinking, on
		LED3_On();
		LED3_Status = 3;
	} else if(LED3_Status == 3) {	// blinking, off
		LED3_Off();
		LED3_Status = 2;
	}
	if(LED4_Status == 0) {				// off
		LED4_Off();
	} else if(LED4_Status == 1) {	// on
		LED4_On();
	} else if(LED4_Status == 2) {	// blinking, on
		LED4_On();
		LED4_Status = 3;
	} else if(LED4_Status == 3) {	// blinking, off
		LED4_Off();
		LED4_Status = 2;
	}
}

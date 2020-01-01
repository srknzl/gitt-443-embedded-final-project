#include "GPIO.h"
#include "SystemStructures.h"

static LED_STATUS LED1_Status = OFF;
static LED_STATUS LED2_Status = ON;
static LED_STATUS LED3_Status = BLINK_ON;
static LED_STATUS LED4_Status = BLINK_OFF;

DeviceStatus* statusref;
static uint32_t turnTempCounter = 0;

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

void GPIO_init(DeviceStatus* s){
	statusref = s;
	IOCON_P1_23 &= ~7;
	IOCON_P1_24 &= ~7;
	IOCON_P0_21 &= ~7;
	
	GPIO_DIR_Write(PORT1,MC_IN1,1);
	GPIO_DIR_Write(PORT1,MC_IN2,1);
	GPIO_DIR_Write(PORT1,MC_SPEED,0);
	
	GPIO_ENR_PORT0 |= 1 << 21;
	GPIO_ENF_PORT0 |= 1 << 21;
	NVIC_EnableIRQ(GPIO_IRQn);
	//GPIO_ADDRESS
		
	//initializes the motor
	PORT1->PIN|=  (1 << 24) | (1 << 23);
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
	uint32_t fallingStatus, risingStatus;
	fallingStatus = GPIO_FALLING_INTERRUPT_STATUS_PORT0;
	risingStatus = GPIO_RISING_INTERRUPT_STATUS_PORT0;
	if((fallingStatus & SPEEDSENSOR_MASK) > 0){
		GPIO_CLEAR_INTERRUPT_PORT0 |= SPEEDSENSOR_MASK;
		turnTempCounter++;
		if(turnTempCounter == 6){
			statusref->turnCount++;
			turnTempCounter = 0;
		}
	}else if((risingStatus & SPEEDSENSOR_MASK) > 0){
		GPIO_CLEAR_INTERRUPT_PORT0 |= SPEEDSENSOR_MASK;
		
	}
}

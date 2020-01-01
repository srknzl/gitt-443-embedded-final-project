#ifndef GPIO_H
#define GPIO_H


#include "LPC407x_8x_177x_8x.h"
#include "DataStructures.h"

typedef struct
{
  volatile	uint32_t DIR;
						uint32_t RESERVED0[3];
  volatile	uint32_t MASK;
  volatile	uint32_t PIN;
  volatile	uint32_t SET;
  volatile  uint32_t CLR;
} GPIO_TypeDef;

typedef enum {
	INPUT = 0,
	OUTPUT = 1
} GPIO_Direction;

typedef enum {
	LOW = 0,
	HIGH = 1
} GPIO_Value;

typedef enum {
	OFF = 0,
	ON = 1,
	BLINK_ON = 2,
	BLINK_OFF = 3
} LED_STATUS;

#define GPIO_ADDRESS	0x20098000

#define GPIO_RISING_INTERRUPT_STATUS_PORT0 *((volatile uint32_t*)(0x40028084))
#define GPIO_FALLING_INTERRUPT_STATUS_PORT0 *((volatile uint32_t*)(0x40028088))
#define GPIO_ENR_PORT0 *((volatile uint32_t*)(0x40028090)) 
#define GPIO_ENF_PORT0 *((volatile uint32_t*)(0x40028094)) 
#define GPIO_CLEAR_INTERRUPT_PORT0 *((volatile uint32_t*)(0x4002808C)) 

#define SPEEDSENSOR_MASK 1<<21

#define PORT0	((GPIO_TypeDef*) PORT0_BASE)
#define PORT1	((GPIO_TypeDef*) PORT1_BASE)
#define PORT2	((GPIO_TypeDef*) PORT2_BASE)
#define PORT3	((GPIO_TypeDef*) PORT3_BASE)
#define PORT4	((GPIO_TypeDef*) PORT4_BASE)
#define PORT5	((GPIO_TypeDef*) PORT5_BASE)

#define PORT0_BASE		(GPIO_ADDRESS + 0x000)
#define PORT1_BASE		(GPIO_ADDRESS + 0x020)
#define PORT2_BASE		(GPIO_ADDRESS + 0x040)
#define PORT3_BASE		(GPIO_ADDRESS + 0x060)
#define PORT4_BASE		(GPIO_ADDRESS + 0x080)
#define PORT5_BASE		(GPIO_ADDRESS + 0x0A0)

//Write Correct Port and Mask values.
#define LED1_PORT			PORT1
#define LED1_MASK			((uint32_t) 1 << 18)

#define LED2_PORT			PORT0
#define LED2_MASK			((uint32_t) 1 << 13)

#define LED3_PORT			PORT1
#define LED3_MASK			((uint32_t) 1 << 13)

#define LED4_PORT			PORT2
#define LED4_MASK			((uint32_t) 1 << 19)

#define IOCON_P0_21 *((volatile uint32_t*)(0x4002C054))
#define IOCON_P1_23 *((volatile uint32_t*)(0x4002C0DC))
#define IOCON_P1_24 *((volatile uint32_t*)(0x4002C0E0))
	
#define MC_IN1 (1 << 23)
#define MC_IN2 (1 << 24)
#define MC_SPEED (1 << 21)

void GPIO_DIR_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value);
void GPIO_PIN_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value);


void LED1_Init(void);
void LED2_Init(void);
void LED3_Init(void);
void LED4_Init(void);

void LED1_Off(void);
void LED2_Off(void);
void LED3_Off(void);
void LED4_Off(void);

void LED1_On(void);
void LED2_On(void);
void LED3_On(void);
void LED4_On(void);

void LED1_Blink(void);
void LED2_Blink(void);
void LED3_Blink(void);
void LED4_Blink(void);

void update_LEDs(void);

void GPIO_init(DeviceStatus* status);

#endif

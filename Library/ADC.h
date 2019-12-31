#ifndef ADC_H
#define ADC_H

#include "LPC407x_8x_177x_8x.h"
#include "Timer.h"
#include "SystemStructures.h"

typedef struct {
  volatile	uint32_t CR;
  volatile	uint32_t GDR;
						uint32_t RESERVED0;
  volatile	uint32_t INTEN;
  volatile	uint32_t DR[8];
  volatile	uint32_t STAT;
  volatile	uint32_t TRM;
} ADC_TypeDef;

#define ADC_CLOCK_FREQUENCY 1000000

#define	LEFT_LDR_ANALOG_PIN_IOCON_ADDRESS	0x4002C064 // P0_25 ADC0[2]
#define LEFT_LDR_ANALOG_PIN_IOCON	*((volatile uint32_t*)(LEFT_LDR_ANALOG_PIN_IOCON_ADDRESS))

#define	RIGHT_LDR_ANALOG_PIN_IOCON_ADDRESS	0x4002C0FC // P1_31 ADC0[5]
#define RIGHT_LDR_ANALOG_PIN_IOCON	*((volatile uint32_t*)(RIGHT_LDR_ANALOG_PIN_IOCON_ADDRESS))

#define	POTENTIOMETER_ANALOG_PIN_IOCON_ADDRESS	0x4002C0F8 // P1_30 ADC0[4]
#define POTENTIOMETER_ANALOG_PIN_IOCON	*((volatile uint32_t*)(POTENTIOMETER_ANALOG_PIN_IOCON_ADDRESS))


//Write the max value of ADC.
#define ADC_MAX_VALUE 0x00000fff

//Define a ADC_CLKDIV variable for given ADC_CLOCK_FREQUENCY.
#define ADC_CLKDIV (PERIPHERAL_CLOCK_FREQUENCY/ADC_CLOCK_FREQUENCY)-1

#define ADC_BASE	0x40034000
#define ADC	((ADC_TypeDef*) ADC_BASE)

extern uint32_t ADC_Last_Light_Left;
extern uint32_t ADC_Last_Light_Right;
extern uint32_t ADC_Last_Speed;
extern uint8_t ADC_New_Left_Light_Available;
extern uint8_t ADC_New_Right_Light_Available;
extern uint8_t ADC_New_Speed_Available;

void ADC_Init(void);
//void ADC_Start(void);
void ADC_Stop(void);
uint32_t ADC_Get_Last_Speed(void);
uint32_t ADC_Get_Last_Left_Light(void);
uint32_t ADC_Get_Last_Right_Light(void);

#endif

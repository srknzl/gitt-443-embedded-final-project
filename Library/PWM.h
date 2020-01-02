#ifndef PWM_H
#define PWM_H

#include "LPC407x_8x_177x_8x.h"
typedef struct
{
  volatile	uint32_t IR;
  volatile	uint32_t TCR;
  volatile	uint32_t TC;
  volatile	uint32_t PR;
  volatile	uint32_t PC;
  volatile	uint32_t MCR;
  volatile	uint32_t MR0;
  volatile	uint32_t MR1;
  volatile	uint32_t MR2;
  volatile	uint32_t MR3;
  volatile	uint32_t CCR;
  volatile  uint32_t CR0;
  volatile  uint32_t CR1;
  volatile  uint32_t CR2;
  volatile  uint32_t CR3;
						uint32_t RESERVED0;
  volatile	uint32_t MR4;
  volatile	uint32_t MR5;
  volatile	uint32_t MR6;
  volatile	uint32_t PCR;
  volatile	uint32_t LER;
						uint32_t RESERVED1[7];
  volatile	uint32_t CTCR;
} PWM_TypeDef;


#define IOCON_MOTOR1_SPEED_ADDRESS	0x4002C094  // P1_5
#define IOCON_MOTOR1_SPEED	*((volatile uint32_t*)(IOCON_MOTOR1_SPEED_ADDRESS))

#define IOCON_MOTOR2_SPEED_ADDRESS	0x4002C098  // P1_6 
#define IOCON_MOTOR2_SPEED	*((volatile uint32_t*)(IOCON_MOTOR2_SPEED_ADDRESS))


#define PWM0_BASE	0x40014000
#define PWM1_BASE	0x40018000

#define PWM0	((PWM_TypeDef*) PWM0_BASE)
#define PWM1	((PWM_TypeDef*) PWM1_BASE)

#define PWMX 	((PWM_TypeDef*) 0x40014000)

void PWM_Init(void);
void PWM_Cycle_Rate(uint32_t period_In_Cycles);
void PWM_Write_Motor2(uint32_t T_ON);
void PWM_Write_Motor1(uint32_t T_ON);

#endif

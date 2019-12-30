#ifndef SYSTICK_H
#define SYSTICK_H

#include "LPC407x_8x_177x_8x.h"

typedef struct
{
  volatile uint32_t SYCTRL;
	volatile uint32_t STRELOAD;
	volatile uint32_t STCURR;
	volatile uint32_t STCALIB;
} STCLK_TypeDef;

#define STCLK_BASE 0xE000E010

#define STCLK ((STCLK_TypeDef*) STCLK_BASE)

// won't be needed
#define STCLK_PIN_IOCON_ADDRESS 0x4002C1E8
#define STCLK_PIN_IOCON *((volatile uint32_t*)(STCLK_PIN_IOCON_ADDRESS))

void SysTick_Init(void);

#endif

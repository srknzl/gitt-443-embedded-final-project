#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "LPC407x_8x_177x_8x.h"
#include "GPIO.h"

typedef enum {
	FORWARD = 0,
	BACKWARD = 1,
	RIGHT = 2,
	LEFT = 3,
	STOP = 4
} Operations;

typedef struct {
	uint32_t distance; // in cms 
	uint16_t lightLevelLeft;
	uint16_t lightLevelRight;
	char* opmode; // AUTO or TEST
	uint32_t speed; // 0-100
	Operations currentOperation; 
	uint8_t underLight; // 1 if light level on either side is more than 300 lumens
	uint8_t willContinue; // Indicates stopped due to light
	uint8_t started; // Start flag in autonomous mode
	uint32_t wheelToothCount; // Turn count for turning
	LED_STATUS LED1_Status;
	LED_STATUS LED2_Status;
	LED_STATUS LED3_Status;
	LED_STATUS LED4_Status;
} DeviceStatus;

#define LIGHT_THRESHOLD 200 
#define TURN_COUNT_FOR_90 6
extern DeviceStatus status;
	
#endif

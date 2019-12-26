#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "GPIO.h"
#include "Timer.h"
#include "SystemStructures.h"

//Write IOCON Register Address of Trigger Pin.
#define IOCON_TRIGGER_ADDRESS	0x4002C024
#define IOCON_TRIGGER	*((volatile uint32_t*)(IOCON_TRIGGER_ADDRESS))

//Write IOCON Register Address of Echo Pin.
#define IOCON_ECHO_ADDRESS	0x4002C060
#define IOCON_ECHO	*((volatile uint32_t*)(IOCON_ECHO_ADDRESS))

extern uint32_t ultrasonicSensorRisingCaptureTime;
extern uint32_t ultrasonicSensorFallingCaptureTime;
extern uint32_t ultrasonicSensorDistance;
extern uint8_t ultrasonicSensorNewDataAvailable;

extern uint8_t ultrasonicSensorTriggerStart;
extern uint8_t ultrasonicSensorCaptureRisingEdge;

void Ultrasonic_Init(void);
void Ultrasonic_Trigger_Timer_Init(void);
void Ultrasonic_Capture_Timer_Init(void);

void Ultrasonic_Start_Trigger_Timer(void);

#endif

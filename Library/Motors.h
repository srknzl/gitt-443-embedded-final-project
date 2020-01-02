#ifndef MOTORS_H
#define MOTORS_H

#include "LPC407x_8x_177x_8x.h"

typedef enum {
	STOPMOVING = 0,
	COUNTERCLOCKWISE = 1,
	CLOCKWISE = 2
}
MotorDirection; 

void Motor1_ChangeDirection(MotorDirection direction);
void Motor2_ChangeDirection(MotorDirection direction);
void Move_Forward(void);
void Move_Backward(void);
void Turn_Left(void);
void Turn_Right(void);
void Stop_Motors(void);
void Change_Motor1_Speed(uint32_t speed);
void Change_Motor2_Speed(uint32_t speed);


#endif

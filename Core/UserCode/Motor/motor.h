#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include "../PID/pid.h"
#include "../user_define.h"
#include "tim.h"

typedef struct
{
	uint32_t ipulse_per_round;
	int32_t icounter;
	float dvelocity;
	float dposition;
	float dreference_velocity;
	float dreference_position;
} Motor_t;

void motor_init(Motor_t *tmotor, uint32_t ipulse);
void motor_reset(Motor_t *motor);
void motor_set_duty(int32_t iduty);
void motor_read_encoder(Motor_t *tmotor, TIM_HandleTypeDef *htim);
void motor_set_velocity(Motor_t *tmotor, PID_CONTROL_t *tpid_ctrl, float dvelocity);
void motor_set_position(Motor_t *tmotor, PID_CONTROL_t *tpid_ctrl, float dposition);

#endif /* INC_MOTOR_H_ */

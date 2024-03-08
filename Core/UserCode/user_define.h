#ifndef USERCODE_USER_DEFINE_H_
#define USERCODE_USER_DEFINE_H_

#include "tim.h"

#define SAMPLING_TIME 0.01f // In second
#define NUMBER_OF_DEGREES_ON_A_CIRCLE 360.0f
#define THOUSAND 1000
#define ZERO 0.0f
// Timer
#define MOTOR1_COUNTER_REGISTER htim4.Instance->CNT
#define MOTOR1_FORWARD_DUTY_CYCLE_REGISTER htim2.Instance->CCR1
#define MOTOR1_BACKWARD_DUTY_CYCLE_REGISTER htim2.Instance->CCR2

#define INTERUPT_TIMER htim3
#define INTERUPT_TIMER_INSTANCE htim3.Instance

#define ECODER_TIMER htim4
#define PWM_TIMER htim2

// motor
#define PPR 6600
#define MAX_VELOCITY 384

// UART
#define MAX_LEN 100
#define UART_COM huart3
#define UART_COM_INSTANCE huart3.Instance

// PID
#define PID_CONTROLLER_LIMIT_MAX (htim2.Init.Period)
#define PID_CONTROLLER_LIMIT_MIN -(float)(htim2.Init.Period)

#define CUT_OFF_FREQUENCY 10

#endif /* USERCODE_USER_DEFINE_H_ */

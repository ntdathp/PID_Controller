#include "motor.h"

void motor_init(Motor_t *tmotor, uint32_t ipulse)
{
    motor_reset(tmotor);
    tmotor->ipulse_per_round = ipulse;
    HAL_TIM_Base_Start_IT(&INTERUPT_TIMER);
    HAL_TIM_Encoder_Start(&ECODER_TIMER, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&ECODER_TIMER, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&PWM_TIMER, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&PWM_TIMER, TIM_CHANNEL_2);
}
void motor_reset(Motor_t *tmotor)
{
    tmotor->icounter = 0;
    tmotor->dvelocity = 0.0f;
    tmotor->dposition = 0.0f;
    tmotor->dreference_velocity = 0.0f;
    tmotor->dreference_position = 0.0f;
}
void motor_set_duty(int32_t iduty)
{
    if (iduty >= 0)
    {
        MOTOR1_FORWARD_DUTY_CYCLE_REGISTER = iduty;
        MOTOR1_BACKWARD_DUTY_CYCLE_REGISTER = 0;
    }
    else
    {
        MOTOR1_FORWARD_DUTY_CYCLE_REGISTER = 0;
        MOTOR1_BACKWARD_DUTY_CYCLE_REGISTER = iduty;
    }
}
void motor_read_encoder(Motor_t *tmotor, TIM_HandleTypeDef *htim)
{
    tmotor->icounter = htim->Instance->CNT;
    tmotor->dvelocity = (float)tmotor->icounter / (float)tmotor->ipulse_per_round * NUMBER_OF_DEGREES_ON_A_CIRCLE / SAMPLING_TIME;
    tmotor->dposition += (float)tmotor->icounter / (float)tmotor->ipulse_per_round * NUMBER_OF_DEGREES_ON_A_CIRCLE;
    htim->Instance->CNT = 0;
}
void motor_set_velocity(Motor_t *tmotor, PID_CONTROL_t *tpid_ctrl, float dvelocity)
{
    tmotor->dreference_velocity = dvelocity;
    motor_set_duty((int)pid_compute(tpid_ctrl, tmotor->dreference_velocity, tmotor->dvelocity));
}
void motor_set_position(Motor_t *tmotor, PID_CONTROL_t *tpid_ctrl, float dposition)
{
    tmotor->dreference_position = dposition;
    motor_set_duty((int)pid_compute(tpid_ctrl, tmotor->dreference_position, tmotor->dposition));
}

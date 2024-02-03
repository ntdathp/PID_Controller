#ifndef INC_PID_H_
#define INC_PID_H_

#include "stdint.h"

// control PID Structure
typedef struct
{
    // Controller gains
    float dkp;
    float dki;
    float dkd;

    // Output limits
    float dlim_min;
    float dlim_max;

    // Intergral limits
    float dlim_max_int;
    float dlim_min_int;

    // Sampling time (in seconds)
    float dts;

    // Controller memory
    float derror;
    float dpre_error;

    // P part, I part, D part
    float dproportional;
    float dintergral;
    float dderivative;

    // Controller output
    float dresult;

} PID_CONTROL_t;

void pid_reset(PID_CONTROL_t *tpid_ctrl);
void pid_init(PID_CONTROL_t *tpid_ctrl, float dkp, float dki, float dkd, float dlimit_max, float dlimit_min, float dts);
void pid_tunning_set(PID_CONTROL_t *tpid_ctrl, float dkp, float dki, float dkd);
float pid_compute(PID_CONTROL_t *tpid_ctrl, float dcmd_value, float dact_value);

#endif /* INC_PID_H_ */

#include "pid.h"

// reset PID params
void pid_reset(PID_CONTROL_t *tpid_ctrl)
{
    tpid_ctrl->dlim_max_int = 0.0f;
    tpid_ctrl->dlim_min_int = 0.0f;

    tpid_ctrl->derror = 0.0f;
    tpid_ctrl->dpre_error = 0.0f;

    tpid_ctrl->dproportional = 0.0f;
    tpid_ctrl->dintergral = 0.0f;
    tpid_ctrl->dderivative = 0.0f;

    tpid_ctrl->dresult = 0.0f;
}

// init PID
void pid_init(PID_CONTROL_t *tpid_ctrl, float dkp, float dki, float dkd, float dlimit_max, float dlimit_min, float dts)
{
    if (dkp < 0.0f || dki < 0.0f || dkp < 0.0f || dts < 0.0f)
    {
        return;
    }

    pid_reset(tpid_ctrl);
    tpid_ctrl->dkp = dkp;
    tpid_ctrl->dki = dki;
    tpid_ctrl->dkd = dkd;

    tpid_ctrl->dlim_max = dlimit_max;
    tpid_ctrl->dlim_min = dlimit_min;

    tpid_ctrl->dts = dts;
}

// set new PID params
void pid_tunning_set(PID_CONTROL_t *tpid_ctrl, float dkp, float dki, float dkd)
{
    if (dkp < 0.0f || dki < 0.0f || dkp < 0.0f)
    {
        return;
    }

    tpid_ctrl->dkp = dkp;
    tpid_ctrl->dki = dki;
    tpid_ctrl->dkd = dkd;
}

// Compute PID Controllers
float pid_compute(PID_CONTROL_t *tpid_ctrl, float dcmd_value, float dact_value)
{
    // Calculate error value
    tpid_ctrl->derror = dcmd_value - dact_value;

    // P part
    tpid_ctrl->dproportional = tpid_ctrl->dkp * tpid_ctrl->derror;

    // I part
    tpid_ctrl->dintergral += 0.5f * tpid_ctrl->dki * tpid_ctrl->dts * (tpid_ctrl->derror + tpid_ctrl->dpre_error);

    // Intergrator anti-wireup

    // Update intergral Limits
    if (tpid_ctrl->dlim_max > tpid_ctrl->dproportional)
    {
        tpid_ctrl->dlim_max_int = tpid_ctrl->dlim_max - tpid_ctrl->dproportional;
    }
    else
    {
        tpid_ctrl->dlim_max_int = 0.0f;
    }
    if (tpid_ctrl->dlim_min < tpid_ctrl->dproportional)
    {
        tpid_ctrl->dlim_min_int = tpid_ctrl->dlim_min - tpid_ctrl->dproportional;
    }
    else
    {
        tpid_ctrl->dlim_min_int = 0.0f;
    }
    // Apply intergral limits
    if (tpid_ctrl->dintergral > tpid_ctrl->dlim_max_int)
    {
        tpid_ctrl->dintergral = tpid_ctrl->dlim_max_int;
    }
    else if (tpid_ctrl->dintergral < tpid_ctrl->dlim_min_int)
    {
        tpid_ctrl->dintergral = tpid_ctrl->dlim_min_int;
    }
    // D part
    tpid_ctrl->dderivative = 2.0f * tpid_ctrl->dkd / tpid_ctrl->dts * (tpid_ctrl->derror - tpid_ctrl->dpre_error) - tpid_ctrl->dderivative;

    // Compute output and apply limits
    tpid_ctrl->dresult = tpid_ctrl->dproportional + tpid_ctrl->dintergral + tpid_ctrl->dderivative;

    if (tpid_ctrl->dresult > tpid_ctrl->dlim_max)
    {
        tpid_ctrl->dresult = tpid_ctrl->dlim_max;
    }
    else if (tpid_ctrl->dresult < tpid_ctrl->dlim_min)
    {
        tpid_ctrl->dresult = tpid_ctrl->dlim_min;
    }

    // Update pre-error
    tpid_ctrl->dpre_error = tpid_ctrl->derror;

    return tpid_ctrl->dresult;
}

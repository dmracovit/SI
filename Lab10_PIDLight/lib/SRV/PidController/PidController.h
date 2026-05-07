#pragma once
#include <Arduino.h>

typedef struct {
    float integral;
    float prevError;
    float dt;          // seconds
    bool  firstRun;
} PidState_t;

void  PidController_Init(PidState_t *s, float dtSec);
void  PidController_Reset(PidState_t *s);

// Compute PID output. Output is unbounded — caller saturates.
//   error = setpoint - measured
//   out = Kp*err + Ki*integral + Kd*derivative
float PidController_Compute(PidState_t *s, float setpoint, float measured,
                            float kp, float ki, float kd,
                            float outMin, float outMax);

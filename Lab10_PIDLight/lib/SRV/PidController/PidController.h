#pragma once
#include <Arduino.h>

// Discrete PID with two enhancements over the textbook form:
//   1) Derivative term is computed on the MEASUREMENT (not the error) so
//      that step-changes of the setpoint do not produce a derivative kick.
//   2) Saturation is handled by BACK-CALCULATION anti-windup, where the
//      excess between the unconstrained and the saturated output is fed
//      back into the integrator at a configurable tracking rate Kt.
typedef struct {
    float    accumI;       // Ki·∫e (already pre-multiplied by Ki)
    float    prevMeas;     // last measurement, for derivative-on-measurement
    float    dtSec;        // sample period in seconds
    uint8_t  primed;       // 0 until prevMeas seeded
} PidController_t;

#define PID_TRACKING_GAIN  1.0f   // back-calculation rate (1/seconds)

void  PidController_Init(PidController_t *ctx, float dtSec);
void  PidController_Reset(PidController_t *ctx);

float PidController_Compute(PidController_t *ctx,
                             float setpoint, float measurement,
                             float kp, float ki, float kd,
                             float outMin, float outMax);

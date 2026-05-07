#include "PidController.h"

void PidController_Init(PidState_t *s, float dtSec)
{
    s->integral  = 0.0f;
    s->prevError = 0.0f;
    s->dt        = (dtSec > 0.0f) ? dtSec : 0.001f;
    s->firstRun  = true;
}

void PidController_Reset(PidState_t *s)
{
    s->integral  = 0.0f;
    s->prevError = 0.0f;
    s->firstRun  = true;
}

// Discrete PID with conditional-integration anti-windup.
//   error      = setpoint - measured
//   derivative = (error - prevError) / dt   (zeroed on first call)
//   tentative  = integral + error*dt
//   raw        = Kp*err + Ki*tentative + Kd*derivative
//   if raw saturates AND error pushes deeper into saturation
//       → freeze integrator (do not commit tentative)
//   else
//       → commit tentative, integral = tentative
float PidController_Compute(PidState_t *s, float setpoint, float measured,
                            float kp, float ki, float kd,
                            float outMin, float outMax)
{
    float error = setpoint - measured;

    float derivative = 0.0f;
    if (!s->firstRun) {
        derivative = (error - s->prevError) / s->dt;
    }

    float tentativeI = s->integral + error * s->dt;
    float raw = kp * error + ki * tentativeI + kd * derivative;

    bool hitHigh = (raw > outMax);
    bool hitLow  = (raw < outMin);

    float clamped = raw;
    if      (hitHigh) clamped = outMax;
    else if (hitLow)  clamped = outMin;

    // Conditional integration: don't keep winding up against saturation.
    bool freezeIntegrator =
        (hitHigh && error > 0.0f) ||
        (hitLow  && error < 0.0f);

    if (!freezeIntegrator) {
        s->integral = tentativeI;
    }

    s->prevError = error;
    s->firstRun  = false;

    return clamped;
}

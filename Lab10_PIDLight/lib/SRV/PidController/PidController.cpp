#include "PidController.h"

void PidController_Init(PidController_t *ctx, float dtSec)
{
    ctx->accumI   = 0.0f;
    ctx->prevMeas = 0.0f;
    ctx->dtSec    = (dtSec > 0.0f) ? dtSec : 0.001f;
    ctx->primed   = 0u;
}

void PidController_Reset(PidController_t *ctx)
{
    ctx->accumI   = 0.0f;
    ctx->prevMeas = 0.0f;
    ctx->primed   = 0u;
}

// Implementation outline:
//   P  =  Kp · (sp - meas)
//   I += Ki · (sp - meas) · dt
//   D  = -Kd · ( meas[k] - meas[k-1] ) / dt   (derivative on measurement)
//   raw = P + I + D
//   out = saturate(raw, outMin, outMax)
//   I  -= Kt · (raw - out) · dt               (back-calculation anti-windup)
float PidController_Compute(PidController_t *ctx,
                             float setpoint, float measurement,
                             float kp, float ki, float kd,
                             float outMin, float outMax)
{
    const float dt    = ctx->dtSec;
    const float error = setpoint - measurement;

    // Proportional term
    const float P = kp * error;

    // Tentatively accumulate the integral (already weighted by Ki)
    ctx->accumI += ki * error * dt;

    // Derivative on the measurement — no spike when the setpoint jumps
    float dMeasure;
    if (ctx->primed == 0u) {
        dMeasure       = 0.0f;
        ctx->primed    = 1u;
    } else {
        dMeasure = (measurement - ctx->prevMeas) / dt;
    }
    ctx->prevMeas = measurement;
    const float D = -kd * dMeasure;   // sign flip because of dM/dt vs de/dt

    // Combine
    const float raw = P + ctx->accumI + D;

    // Saturate the output
    float out = raw;
    if (out > outMax) out = outMax;
    if (out < outMin) out = outMin;

    // Back-calculation: pull integrator back proportionally to the excess
    const float excess = raw - out;
    if (excess != 0.0f) {
        ctx->accumI -= PID_TRACKING_GAIN * excess * dt;
    }

    return out;
}

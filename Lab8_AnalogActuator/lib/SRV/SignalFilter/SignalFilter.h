#pragma once
#include <Arduino.h>

#define MEDIAN_WINDOW 5
#define EMA_ALPHA     0.3f

typedef struct {
    float    medianBuf[MEDIAN_WINDOW];
    uint8_t  medianIdx;
    uint8_t  medianCount;
    float    emaValue;
    bool     emaInit;
} FilterState_t;

void  SignalFilter_Init(FilterState_t *state);

// Full pipeline: saturate → median → EMA
float SignalFilter_Process(FilterState_t *state, float rawValue, float satMin, float satMax);

// Individual stages
float SignalFilter_Saturate(float value, float min, float max);
float SignalFilter_Median(FilterState_t *state, float value);
float SignalFilter_EMA(FilterState_t *state, float value);

// Ramp: move current toward target by at most maxStep
float SignalFilter_Ramp(float current, float target, float maxStep);

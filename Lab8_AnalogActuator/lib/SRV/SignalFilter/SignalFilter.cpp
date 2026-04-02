#include "SignalFilter.h"
#include <string.h>

void SignalFilter_Init(FilterState_t *state)
{
    memset(state, 0, sizeof(FilterState_t));
}

float SignalFilter_Saturate(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float SignalFilter_Median(FilterState_t *state, float value)
{
    state->medianBuf[state->medianIdx] = value;
    state->medianIdx = (state->medianIdx + 1) % MEDIAN_WINDOW;
    if (state->medianCount < MEDIAN_WINDOW)
        state->medianCount++;

    // Copy and sort
    float sorted[MEDIAN_WINDOW];
    uint8_t n = state->medianCount;
    memcpy(sorted, state->medianBuf, n * sizeof(float));

    for (uint8_t i = 1; i < n; i++) {
        float key = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > key) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }

    return sorted[n / 2];
}

float SignalFilter_EMA(FilterState_t *state, float value)
{
    if (!state->emaInit) {
        state->emaValue = value;
        state->emaInit = true;
        return value;
    }
    state->emaValue = EMA_ALPHA * value + (1.0f - EMA_ALPHA) * state->emaValue;
    return state->emaValue;
}

float SignalFilter_Process(FilterState_t *state, float rawValue, float satMin, float satMax)
{
    float sat = SignalFilter_Saturate(rawValue, satMin, satMax);
    float med = SignalFilter_Median(state, sat);
    float ema = SignalFilter_EMA(state, med);
    return ema;
}

float SignalFilter_Ramp(float current, float target, float maxStep)
{
    float diff = target - current;
    if (diff >  maxStep) return current + maxStep;
    if (diff < -maxStep) return current - maxStep;
    return target;  // close enough, snap to target
}

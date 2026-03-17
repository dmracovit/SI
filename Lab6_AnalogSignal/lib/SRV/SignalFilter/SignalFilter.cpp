#include "SignalFilter.h"
#include <string.h>

void SignalFilter_Init(FilterState_t *state)
{
    memset(state, 0, sizeof(FilterState_t));
    state->emaInit = false;
}

// ===== Stage 1: Saturation =====
// Clamps value to [min, max] range. Prevents out-of-range readings
// from corrupting downstream filters.
float SignalFilter_Saturate(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// ===== Stage 2: Median Filter (salt & pepper removal) =====
// Keeps a sliding window of MEDIAN_WINDOW values.
// Returns the median — immune to single-sample spikes/drops.
float SignalFilter_Median(FilterState_t *state, float value)
{
    // Add new value to circular buffer
    state->medianBuf[state->medianIdx] = value;
    state->medianIdx = (state->medianIdx + 1) % MEDIAN_WINDOW;
    if (state->medianCount < MEDIAN_WINDOW)
        state->medianCount++;

    // Copy current samples and sort (insertion sort — small N)
    float sorted[MEDIAN_WINDOW];
    uint8_t n = state->medianCount;
    memcpy(sorted, state->medianBuf, n * sizeof(float));

    for (uint8_t i = 1; i < n; i++)
    {
        float key = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > key)
        {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }

    // Return middle element
    return sorted[n / 2];
}

// ===== Stage 3: Exponential Moving Average (EMA) =====
// Weighted average: new_ema = alpha * value + (1 - alpha) * old_ema
// Smooths noise while tracking trends.
float SignalFilter_EMA(FilterState_t *state, float value)
{
    if (!state->emaInit)
    {
        state->emaValue = value; // first sample: no history
        state->emaInit = true;
        return value;
    }

    state->emaValue = EMA_ALPHA * value + (1.0f - EMA_ALPHA) * state->emaValue;
    return state->emaValue;
}

// ===== Full pipeline =====
float SignalFilter_Process(FilterState_t *state, float rawValue, float satMin, float satMax)
{
    float saturated = SignalFilter_Saturate(rawValue, satMin, satMax);
    float median    = SignalFilter_Median(state, saturated);
    float ema       = SignalFilter_EMA(state, median);
    return ema;
}

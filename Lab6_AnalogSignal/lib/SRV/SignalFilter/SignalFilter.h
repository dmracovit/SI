#pragma once
#include <Arduino.h>

// --- Filter configuration ---
#define MEDIAN_WINDOW   5   // median filter window size (must be odd)
#define EMA_ALPHA       0.3f // exponential moving average weight (0..1, higher = more responsive)

// --- Saturation limits ---
#define TEMP_SAT_MIN    -10.0f
#define TEMP_SAT_MAX    60.0f
#define SOUND_SAT_MIN   0
#define SOUND_SAT_MAX   4095

// Filter state for one channel (holds history for median + EMA)
typedef struct {
    float   medianBuf[MEDIAN_WINDOW]; // circular buffer for median filter
    uint8_t medianIdx;                // current write index
    uint8_t medianCount;              // number of samples collected (up to MEDIAN_WINDOW)
    float   emaValue;                 // current EMA output
    bool    emaInit;                  // has EMA been initialized?
} FilterState_t;

// Initialize filter state
void SignalFilter_Init(FilterState_t *state);

// Full pipeline: saturate → median → weighted average (EMA)
// Returns the filtered value
float SignalFilter_Process(FilterState_t *state, float rawValue, float satMin, float satMax);

// Individual stages (exposed for reporting intermediate values)
float SignalFilter_Saturate(float value, float min, float max);
float SignalFilter_Median(FilterState_t *state, float value);
float SignalFilter_EMA(FilterState_t *state, float value);

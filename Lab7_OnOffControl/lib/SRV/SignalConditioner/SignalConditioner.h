#pragma once
#include <Arduino.h>

typedef struct {
    int  lastRaw;            // last raw input value
    int  debounced;          // value after debounce stage
    int  validated;          // value after validation stage (final output)
    unsigned long debounceStart;
    unsigned long validateStart;
    bool firstRun;
} CondState_t;

void SignalConditioner_Init(CondState_t *state);

// Process raw input through debounce + validation.
// Returns validated (confirmed) value.
int  SignalConditioner_Process(CondState_t *state, int rawInput,
                               unsigned long debounceMs, unsigned long validationMs);

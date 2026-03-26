#include "SignalConditioner.h"

void SignalConditioner_Init(CondState_t *state)
{
    state->lastRaw       = 0;
    state->debounced     = 0;
    state->validated     = 0;
    state->debounceStart = 0;
    state->validateStart = 0;
    state->firstRun      = true;
}

int SignalConditioner_Process(CondState_t *state, int rawInput,
                              unsigned long debounceMs, unsigned long validationMs)
{
    unsigned long now = millis();

    // First run: accept value immediately
    if (state->firstRun) {
        state->lastRaw       = rawInput;
        state->debounced     = rawInput;
        state->validated     = rawInput;
        state->debounceStart = now;
        state->validateStart = now;
        state->firstRun      = false;
        return rawInput;
    }

    // --- Stage 1: Debounce ---
    // If raw input changed, restart debounce timer
    if (rawInput != state->lastRaw) {
        state->lastRaw       = rawInput;
        state->debounceStart = now;
    }

    // If raw input stable for debounceMs → accept as debounced
    if ((now - state->debounceStart) >= debounceMs) {
        if (state->lastRaw != state->debounced) {
            state->debounced     = state->lastRaw;
            state->validateStart = now;  // start validation
        }
    }

    // --- Stage 2: Validation ---
    // If debounced value differs from validated, wait for validationMs
    if (state->debounced != state->validated) {
        // Check if debounced value changed during validation → restart
        if ((now - state->validateStart) >= validationMs) {
            state->validated = state->debounced;
        }
    }

    return state->validated;
}

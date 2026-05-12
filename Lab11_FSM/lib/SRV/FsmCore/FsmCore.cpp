#include "FsmCore.h"
#include "LedDriver.h"
#include "AppState.h"
#include <stddef.h>

// Table-driven FSM: each row is one allowed transition.
// Anything not listed leaves the state unchanged.
typedef struct {
    FsmState_t from;
    FsmEvent_t on;
    FsmState_t to;
} FsmEdge_t;

static const FsmEdge_t TRANSITIONS[] = {
    { FSM_IDLE,   FSM_EV_SHORT_PRESS, FSM_ARMED  },
    { FSM_IDLE,   FSM_EV_LONG_PRESS,  FSM_LOCKED },

    { FSM_ARMED,  FSM_EV_SHORT_PRESS, FSM_ACTIVE },
    { FSM_ARMED,  FSM_EV_LONG_PRESS,  FSM_IDLE   },
    { FSM_ARMED,  FSM_EV_TIMEOUT,     FSM_IDLE   },

    { FSM_ACTIVE, FSM_EV_SHORT_PRESS, FSM_IDLE   },
    { FSM_ACTIVE, FSM_EV_LONG_PRESS,  FSM_LOCKED },

    { FSM_LOCKED, FSM_EV_LONG_PRESS,  FSM_IDLE   },
};
#define EDGE_COUNT  (sizeof(TRANSITIONS) / sizeof(TRANSITIONS[0]))

static FsmState_t currentState = FSM_IDLE;

// Animation / timer counters live in the FSM module — the LED is driven
// purely by state so this is Moore-style (output = f(state) only).
static uint16_t armedTimer    = 0;
static uint8_t  blinkDivider  = 0;
static bool     blinkPhase    = false;

// Per-state on-entry action. Resets timers, sets steady LED level.
static void onStateEntry(FsmState_t s)
{
    currentState = s;
    armedTimer   = 0;
    blinkDivider = 0;
    blinkPhase   = false;
    switch (s) {
        case FSM_IDLE:   LedDriver_Off(); break;
        case FSM_ARMED:  LedDriver_Off(); break;  // first half of slow blink
        case FSM_ACTIVE: LedDriver_On();  break;
        case FSM_LOCKED: LedDriver_On();  break;  // first half of fast blink
    }
}

void FsmCore_Init(void)
{
    onStateEntry(FSM_IDLE);
}

FsmState_t FsmCore_GetState(void)
{
    return currentState;
}

const char *FsmCore_StateName(FsmState_t s)
{
    switch (s) {
        case FSM_IDLE:   return "IDLE";
        case FSM_ARMED:  return "ARMED";
        case FSM_ACTIVE: return "ACTIVE";
        case FSM_LOCKED: return "LOCKED";
        default:         return "?";
    }
}

const char *FsmCore_EventName(FsmEvent_t e)
{
    switch (e) {
        case FSM_EV_SHORT_PRESS: return "short";
        case FSM_EV_LONG_PRESS:  return "long";
        case FSM_EV_TIMEOUT:     return "timeout";
        case FSM_EV_TICK:        return "tick";
        default:                 return "none";
    }
}

// Periodic side-effects: blink animation and ARMED-timeout countdown.
// Returns an additional synthetic event (TIMEOUT) when ARMED elapses;
// otherwise NONE. Animation happens regardless.
static FsmEvent_t periodicTickActions(void)
{
    FsmEvent_t synth = FSM_EV_NONE;
    switch (currentState) {
        case FSM_ARMED:
            // Slow blink: toggle every 6 ticks (≈300 ms half-period).
            if (++blinkDivider >= 6) {
                blinkDivider = 0;
                blinkPhase   = !blinkPhase;
                LedDriver_Set(blinkPhase);
            }
            if (++armedTimer >= ARMED_TIMEOUT_TICKS) {
                synth = FSM_EV_TIMEOUT;
            }
            break;
        case FSM_LOCKED:
            // Fast blink: toggle every 2 ticks (≈100 ms half-period).
            if (++blinkDivider >= 2) {
                blinkDivider = 0;
                blinkPhase   = !blinkPhase;
                LedDriver_Set(blinkPhase);
            }
            break;
        default:
            break;
    }
    return synth;
}

// Look up the destination state for the (current, event) pair.
// Returns the same state if no transition is defined.
static FsmState_t lookupTransition(FsmState_t from, FsmEvent_t ev)
{
    for (size_t i = 0; i < EDGE_COUNT; ++i) {
        if (TRANSITIONS[i].from == from && TRANSITIONS[i].on == ev) {
            return TRANSITIONS[i].to;
        }
    }
    return from;
}

FsmState_t FsmCore_Step(FsmEvent_t ev)
{
    // Periodic tick: run animation and possibly inject a synthetic TIMEOUT
    // event for the ARMED state.
    if (ev == FSM_EV_TICK) {
        const FsmEvent_t synth = periodicTickActions();
        if (synth == FSM_EV_NONE) return currentState;
        ev = synth;
    }

    const FsmState_t next = lookupTransition(currentState, ev);
    if (next != currentState) {
        printf("[FSM] %-6s --(%s)--> %s\n",
               FsmCore_StateName(currentState),
               FsmCore_EventName(ev),
               FsmCore_StateName(next));
        AppState_BumpTransitions();
        onStateEntry(next);
    }
    return currentState;
}

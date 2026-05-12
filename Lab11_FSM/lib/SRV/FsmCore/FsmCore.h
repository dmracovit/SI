#pragma once
#include <Arduino.h>

// ── 4-state Moore-style FSM, table-driven ─────────────────────────────
//
// States:
//   IDLE     — quiet; the system is doing nothing.
//   ARMED    — waiting for confirmation; LED slow-blinks.
//              Auto-reverts to IDLE after ARMED_TIMEOUT_TICKS without input.
//   ACTIVE   — LED is steady on.
//   LOCKED   — alarm-like rapid blink; can be escaped ONLY by long press.
//
// Input alphabet:
//   EV_SHORT_PRESS — momentary tap on the joystick
//   EV_LONG_PRESS  — held >= LONG_PRESS_MS
//   EV_TICK        — periodic 50 ms tick (drives blink animation + timeout)
//   EV_TIMEOUT     — internal, emitted when ARMED runs its timer down
//
// Transition table (rows that are not listed = stay in the same state):
//
//      ┌────────┬───────────────┬───────────────┬──────────────┐
//      │ State  │  SHORT        │  LONG         │  TIMEOUT     │
//      ├────────┼───────────────┼───────────────┼──────────────┤
//      │ IDLE   │  ARMED        │  LOCKED       │  -           │
//      │ ARMED  │  ACTIVE       │  IDLE         │  IDLE        │
//      │ ACTIVE │  IDLE         │  LOCKED       │  -           │
//      │ LOCKED │  -            │  IDLE         │  -           │
//      └────────┴───────────────┴───────────────┴──────────────┘

typedef enum {
    FSM_IDLE   = 0,
    FSM_ARMED  = 1,
    FSM_ACTIVE = 2,
    FSM_LOCKED = 3
} FsmState_t;

typedef enum {
    FSM_EV_NONE        = 0,
    FSM_EV_SHORT_PRESS = 1,
    FSM_EV_LONG_PRESS  = 2,
    FSM_EV_TICK        = 3,
    FSM_EV_TIMEOUT     = 4
} FsmEvent_t;

// Ticks (= FSM task periods, 50 ms each) before ARMED auto-reverts to IDLE.
#define ARMED_TIMEOUT_TICKS   (5000 / 50)   // 5 s

void         FsmCore_Init(void);
FsmState_t   FsmCore_GetState(void);
FsmState_t   FsmCore_Step(FsmEvent_t ev);
const char  *FsmCore_StateName(FsmState_t s);
const char  *FsmCore_EventName(FsmEvent_t e);

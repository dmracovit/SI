#pragma once

#define RELAY_PIN 23

void RelayDriver_Init(void);
void RelayDriver_On(void);
void RelayDriver_Off(void);
void RelayDriver_Set(bool on);
bool RelayDriver_GetState(void);

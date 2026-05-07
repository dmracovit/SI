#pragma once

#define RELAY_PIN        7
#define RELAY_ACTIVE_LOW 1   // 5V relay module is active-LOW

void RelayDriver_Init(void);
void RelayDriver_Set(bool on);
bool RelayDriver_GetState(void);

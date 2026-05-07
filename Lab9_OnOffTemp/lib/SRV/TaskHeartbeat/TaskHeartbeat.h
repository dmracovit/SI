#pragma once
#include <Arduino_FreeRTOS.h>
#include <task.h>

#define HEARTBEAT_PIN          13
#define HEARTBEAT_PERIOD_MS   500

void TaskHeartbeat_Task(void *pvParameters);

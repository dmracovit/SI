#include "ServoDriver.h"

static float currentAngle = 0.0f;

void ServoDriver_Init(void)
{
    ledcSetup(SERVO_PWM_CH, SERVO_PWM_FREQ, SERVO_PWM_RES);
    ledcAttachPin(SERVO_PIN, SERVO_PWM_CH);
    ServoDriver_SetAngle(90.0f);  // start at center
}

void ServoDriver_SetAngle(float angle)
{
    // Clamp to valid range
    if (angle < SERVO_MIN_ANGLE) angle = SERVO_MIN_ANGLE;
    if (angle > SERVO_MAX_ANGLE) angle = SERVO_MAX_ANGLE;

    currentAngle = angle;

    // Map angle to pulse width in microseconds
    float pulseUs = SERVO_MIN_US + (angle / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US);

    // Convert to duty cycle (16-bit, 50Hz → period = 20000us)
    uint32_t duty = (uint32_t)((pulseUs / 20000.0f) * 65535.0f);
    ledcWrite(SERVO_PWM_CH, duty);
}

float ServoDriver_GetAngle(void)
{
    return currentAngle;
}

#include "CommandParser.h"
#include "LedControl.h"
#include "SerialIO.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static bool blinkActive = false;
static unsigned long lastBlinkTime = 0;
#define BLINK_INTERVAL 500

void CommandParserInit(void)
{
    blinkActive = false;
    lastBlinkTime = 0;
}

static void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

static void trimString(char *str)
{
    str[strcspn(str, "\r\n")] = '\0';

    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1]))
    {
        str[--len] = '\0';
    }
}

void CommandParserExecute(const char *command)
{
    char cmd[32];
    strncpy(cmd, command, sizeof(cmd) - 1);
    cmd[sizeof(cmd) - 1] = '\0';

    toLowerCase(cmd);
    trimString(cmd);

    char target[16] = "";
    char action[16] = "";
    sscanf(cmd, "%15s %15s", target, action);

    if (strcmp(target, "led") == 0)
    {
        if (strcmp(action, "on") == 0)
        {
            blinkActive = false;
            LedOn();
            SerialPrint("OK: LED turned ON\n");
        }
        else if (strcmp(action, "off") == 0)
        {
            blinkActive = false;
            LedOff();
            SerialPrint("OK: LED turned OFF\n");
        }
        else if (strcmp(action, "blink") == 0)
        {
            blinkActive = true;
            lastBlinkTime = millis();
            SerialPrint("OK: LED blinking (500ms interval)\n");
        }
        else
        {
            SerialPrint("ERROR: Unknown LED action '%s'\n", action);
            SerialPrint("Valid actions: 'on', 'off', 'blink'\n");
        }
    }
    else if (strlen(cmd) > 0)
    {
        SerialPrint("ERROR: Unknown command '%s'\n", cmd);
        SerialPrint("Valid commands: 'led on', 'led off', 'led blink'\n");
    }
}

void CommandParserUpdate(void)
{
    if (blinkActive && (millis() - lastBlinkTime >= BLINK_INTERVAL))
    {
        LedToggle();
        lastBlinkTime = millis();
    }
}

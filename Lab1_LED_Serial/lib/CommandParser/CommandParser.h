#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <Arduino.h>

void CommandParserInit(void);
void CommandParserExecute(const char *command);
void CommandParserUpdate(void);

#endif

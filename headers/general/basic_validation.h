#ifndef BASIC_VALIDATION_H
#define BASIC_VALIDATION_H

#include "../types.h"

void add_error(Response *response, char *message);
Response is_command_valid(ParsedCommand parsedCommand);

#endif
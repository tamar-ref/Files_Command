#ifndef VALIDATION_H
#define VALIDATION_H

#include "../types.h"

void is_forbidden_path(char *filename, Response *response);
void is_file_exists(ParsedCommand parsedCommand, Response *response);
Response validation(ParsedCommand parsedCommand);

#endif
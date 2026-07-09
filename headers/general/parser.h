#ifndef PARSER_H
#define PARSER_H

#include "../types.h"

int copy_token(char *input, char *destination, char *delimiters, int token_size);
ParsedCommand parser(char *input);

#endif
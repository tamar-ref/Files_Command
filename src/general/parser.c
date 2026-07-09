#include <stdio.h>
#include <string.h>

#include "../../headers/types.h"

int copy_token(char *input, char *destination, char *delimiters, int token_size)
{
    char *token = strtok(input, delimiters);
    if (token)
    {
        strncpy(destination, token, token_size - 1);
    }
}

ParsedCommand parser(char *input)
{
    ParsedCommand parsedCommand = {0};
    input[strcspn(input, "\n")] = '\0';

    copy_token(input, parsedCommand.command, " \t", sizeof(parsedCommand.command));
    copy_token(NULL, parsedCommand.filename, " \t", sizeof(parsedCommand.filename));
    copy_token(NULL, parsedCommand.content, "", sizeof(parsedCommand.content));

    return parsedCommand;
}
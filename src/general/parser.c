#include <stdio.h>
#include <string.h>

#include "../../headers/types.h"

#define BUFFER_SIZE 1024

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
    char temp_input[BUFFER_SIZE];
    strcpy(temp_input, input);
    temp_input[strcspn(temp_input, "\n")] = '\0';

    copy_token(temp_input, parsedCommand.command, " \t", sizeof(parsedCommand.command));
    copy_token(NULL, parsedCommand.filename, " \t", sizeof(parsedCommand.filename));
    copy_token(NULL, parsedCommand.content, "", sizeof(parsedCommand.content));

    return parsedCommand;
}
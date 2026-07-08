#include <stdio.h>
#include <string.h>

#include "types.h"

ParsedCommand parser(char *input)
{
    ParsedCommand parsedCommand;
    // איפוס המבנה
    memset(&parsedCommand, 0, sizeof(ParsedCommand));
    // ניקוי ירידת השורה מהקלט המקורי
    input[strcspn(input, "\n")] = '\0';

    // command
    char *token = strtok(input, " \t");
    if (token != NULL)
    {
        strncpy(parsedCommand->command, token, sizeof(parsedCommand->command) - 1);
    }
    else
    {
        return;
    }

    // file name
    token = strtok(NULL, " \t");
    if (token != NULL)
    {
        strncpy(parsedCommand->filename, token, sizeof(parsedCommand->filename) - 1);
    }
    else
    {
        return;
    }

    // content
    token = strtok(NULL, "");
    if (token != NULL)
    {
        strncpy(parsedCommand->content, token, sizeof(parsedCommand->content) - 1);
    }
    
    return parsedCommand;
}
#include <stdio.h>
#include <string.h>

#include "../../headers/types.h"

#define BUFFER_SIZE 1024

ParsedCommand parser(char *input, int filename_size)
{
    ParsedCommand parsedCommand = {0};
    char temp_input[BUFFER_SIZE];
    strcpy(temp_input, input);
    temp_input[strcspn(temp_input, "\n")] = '\0';

    char *token = strtok(temp_input, " \t");
    if (token)
    {
        strncpy(parsedCommand.command, token, sizeof(parsedCommand.command) - 1);
    }

    char *filename_start = strtok(NULL, "");

    if (filename_start)
    {
        while (*filename_start == ' ' || *filename_start == '\t')
        {
            filename_start++;
        }
        strncpy(parsedCommand.filename, filename_start, filename_size);
        parsedCommand.filename[filename_size] = '\0';
        char *content_start = filename_start + filename_size;
        while (*content_start == ' ' || *content_start == '\t')
        {
            content_start++;
        }
        strncpy(parsedCommand.content, content_start, sizeof(parsedCommand.content) - 1);
    }

    return parsedCommand;
}
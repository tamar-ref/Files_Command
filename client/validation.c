#include "../types.h"

void add_error(Response *response, char *message)
{
    if (response->error_count >= MAX_ERRORS)
        return;

    strncpy(
        response->errors[response->error_count],
        message,
        ERROR_LENGTH - 1);

    response->errors[response->error_count][ERROR_LENGTH - 1] = '\0';
    response->error_count++;
}

Response validation(ParsedCommand parsedCommand)
{
    Response response={0};
    
    if (strlen(parsedCommand.command) == 0)
    {
        add_error(&response, "command is missing");
    }
    if (strlen(parsedCommand.filename) == 0)
    {
        add_error(&response, "file name is missing");
    }

    char *commandsWithContent[] = {"WRITE", "APPEND"};
    char *commandsWithoutContent[] = {"CREATE", "READ", "DELETE"};
    int known_command_flag = 0;

    for (int i = 0; i < 2 && known_command_flag == 0; i++)
    {
        if (strcmp(parsedCommand.command, commandsWithContent[i]) == 0)
        {
            known_command_flag = 1;
            if (strlen(parsedCommand.content) == 0)
            {
                add_error(&response, "content is missing");
            }
        }
    }
    for (int i = 0; i < 3 && known_command_flag == 0; i++)
    {
        if (strcmp(parsedCommand.command, commandsWithoutContent[i]) == 0)
        {
            known_command_flag = 1;
            if (strlen(parsedCommand.content) != 0)
            {
                add_error(&response, "too many arguments");
            }
        }
    }

    if (known_command_flag == 0)
    {
        add_error(&response, "invalid command");
    }

    return response;
}
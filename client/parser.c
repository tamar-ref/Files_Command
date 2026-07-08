#include <stdio.h>
#include <string.h>

#include "types.c"

void build_struct(ParsedCommand *parsedCommand, char *input)
{
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
}

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

void command_type(Response *response)
{
    if (strlen(response->command.command) == 0)
    {
        add_error(response, "command is missing");
        strncpy(response->result, "error", sizeof(response->result) - 1);
    }
    if (strlen(response->command.filename) == 0)
    {
        add_error(response, "file name is missing");
        strncpy(response->result, "error", sizeof(response->result) - 1);
    }

    char *commandsWithContent[] = {"WRITE", "APPEND"};
    char *commandsWithoutContent[] = {"CREATE", "READ", "DELETE"};
    int flag = 0;

    for (int i = 0; i < 2 && flag == 0; i++)
    {
        if (strcmp(response->command.command, commandsWithContent[i]) == 0)
        {
            flag = 1;
            if (strlen(response->command.content) == 0)
            {
                add_error(response, "content is missing");
                strncpy(response->result, "error", sizeof(response->result) - 1);
            }
        }
    }
    for (int i = 0; i < 3 && flag == 0; i++)
    {
        if (strcmp(response->command.command, commandsWithoutContent[i]) == 0)
        {
            flag = 1;
            if (strlen(response->command.content) != 0)
            {
                add_error(response, "too many arguments");
                strncpy(response->result, "error", sizeof(response->result) - 1);
            }
        }
    }

    if (flag == 0)
    {
        add_error(response, "invalid command");
        strncpy(response->result, "error", sizeof(response->result) - 1);
    }
}

Response parser(char *input)
{
    Response response;

    // איפוס המבנה
    memset(&response, 0, sizeof(Response));
    strncpy(response.result, "success", sizeof(response.result) - 1);

    // ניקוי ירידת השורה מהקלט המקורי
    input[strcspn(input, "\n")] = '\0';

    build_struct(&response.command, input);
    command_type(&response);

    return response;
}
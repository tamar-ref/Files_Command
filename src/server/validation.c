#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "../../headers/types.h"
#include "../../headers/general/basic_validation.h"

void is_forbidden_path(char *filename, Response *response)
{
    if (strchr(filename, '/'))
    {
        add_error(response, "cannot access a file external to the output directory");
    }
}

void is_file_exists(ParsedCommand parsedCommand, Response *response)
{
    char output_folder[] = "../output/";
    char file[sizeof output_folder + sizeof parsedCommand.filename] = {0};

    strcat(file, output_folder);
    strcat(file, parsedCommand.filename);

    if (strcmp(parsedCommand.command, "CREATE") == 0 && access(file, F_OK) == 0)
    {
        add_error(response, "file name already exists");
    }
    else if (strcmp(parsedCommand.command, "CREATE") != 0 && access(file, F_OK) != 0)
    {
        add_error(response, "file not found");
    }
}

Response validation(ParsedCommand parsedCommand)
{
    Response response = {0};
    is_forbidden_path(parsedCommand.filename, &response);
    is_file_exists(parsedCommand, &response);
    return response;
}
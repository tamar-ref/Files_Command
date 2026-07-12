#include <stdio.h>
#include <string.h>

#include "../../headers/types.h"
#include "../../headers/general/basic_validation.h"

#define BUFFER_SIZE 1024

void create_file(ParsedCommand parsedCommand, Response *response)
{
    FILE *file;
    char path[200] = "../output/";
    strcat(path, parsedCommand.filename);
    file = fopen(path, "w");
    if (file == NULL)
    {
        add_error(response, "could not create file");
        return;
    }
    fclose(file);
    strcpy(response->success_message, "file created successfully\n");
}

void read_file(ParsedCommand parsedCommand, Response *response)
{
    FILE *file;
    char path[200] = "../output/";
    strcat(path, parsedCommand.filename);
    file = fopen(path, "r");
    if (file == NULL)
    {
        add_error(response, "could not read file");
        return;
    }
    memset(response->success_message, 0, BUFFER_SIZE);
    int bytes_read = fread(response->success_message, 1, BUFFER_SIZE - 1, file);
    response->success_message[bytes_read] = '\n';
    response->success_message[bytes_read + 1] = '\0';
    fclose(file);
}

void write_to_file(ParsedCommand parsedCommand, Response *response)
{
    FILE *file;
    char path[200] = "../output/";
    strcat(path, parsedCommand.filename);
    file = fopen(path, "w");
    if (file == NULL)
    {
        add_error(response, "could not open file for writing");
        return;
    }
    fprintf(file, "%s", parsedCommand.content);
    fclose(file);
    strcpy(response->success_message, "written successfully\n");
}

void append_to_file(ParsedCommand parsedCommand, Response *response)
{
    FILE *file;
    char path[200] = "../output/";
    strcat(path, parsedCommand.filename);
    file = fopen(path, "a");
    if (file == NULL)
    {
        add_error(response, "could not open file for appending");
        return;
    }
    fprintf(file, "%s", parsedCommand.content);
    fclose(file);
    strcpy(response->success_message, "appended content successfully\n");
}

void delete_file(ParsedCommand parsedCommand, Response *response)
{
    char path[200] = "../output/";
    strcat(path, parsedCommand.filename);
    if (remove(path) == 0)
    {
        strcpy(response->success_message, "file deleted successfully\n");
    }
    else
    {
        add_error(response, "could not delete file");
    }
}

Response file_manager(ParsedCommand parsedCommand)
{
    Response response = {0};

    if (strcmp(parsedCommand.command, "CREATE") == 0)
    {
        create_file(parsedCommand, &response);
    }
    else if (strcmp(parsedCommand.command, "READ") == 0)
    {
        read_file(parsedCommand, &response);
    }
    else if (strcmp(parsedCommand.command, "WRITE") == 0)
    {
        write_to_file(parsedCommand, &response);
    }
    else if (strcmp(parsedCommand.command, "APPEND") == 0)
    {
        append_to_file(parsedCommand, &response);
    }
    else if (strcmp(parsedCommand.command, "DELETE") == 0)
    {
        delete_file(parsedCommand, &response);
    }

    return response;
}
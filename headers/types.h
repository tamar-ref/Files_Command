#ifndef TYPES_H
#define TYPES_H

#define MAX_ERRORS 30
#define MESSAGE_LENGTH 100

typedef struct
{
    char command[10];
    char filename[100];
    char content[500];
} ParsedCommand;

typedef struct
{
    char errors[MAX_ERRORS][MESSAGE_LENGTH];
    int error_count;
    char success_message[MESSAGE_LENGTH];
} Response;

#endif
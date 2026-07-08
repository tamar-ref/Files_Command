#ifndef TYPES_H
#define TYPES_H

#define MAX_ERRORS 10
#define ERROR_LENGTH 100

typedef struct
{
    char command[10];
    char filename[100];
    char content[500];
} ParsedCommand;

typedef struct
{
    char errors[MAX_ERRORS][ERROR_LENGTH];
    int error_count;
} Response;

#endif
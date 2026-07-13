#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "../../headers/types.h"
#include "../../headers/general/parser.h"
#include "../../headers/general/basic_validation.h"
#include "../../headers/general/functions.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define LOCALHOST_ADDRESS "127.0.0.1"

int print_response(char *message, Response response)
{
    if (response.error_count == 0)
        return 0;

    set_message(message, response);
    printf("%s", message);

    return 1;
}

int connect_to_server()
{
    int sock;
    struct sockaddr_in server_addr = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        perror("socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, LOCALHOST_ADDRESS, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        return -1;
    }

    return sock;
}

int get_input(char *text, char *input_buffer, int input_size)
{
    printf("%s\n", text);
    fgets(input_buffer, input_size, stdin);

    if (input_buffer[strlen(input_buffer) - 1] != '\n')
    {
        printf("Error: input is too long. max allowed size is %d characters.\n", input_size - 2);

        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return -1;
    }

    if (input_buffer[0] == '\n')
    {
        printf("Empty input. Please enter a valid command.\n");
        return -1;
    }
    return 1;
}

int main()
{
    int sock, result, filename_size;
    char input_buffer[BUFFER_SIZE] = {0};
    char response_buffer[BUFFER_SIZE] = {0};
    Response response;
    char message[BUFFER_SIZE] = {0};

    sock = connect_to_server();
    if (sock == -1)
    {
        return 1;
    }

    while (1)
    {
        memset(input_buffer, 0, sizeof(input_buffer));
        memset(message, 0, sizeof(message));

        if (get_input("Enter command: ", input_buffer, sizeof(input_buffer)) == -1)
        {
            continue;
        }
        while (1)
        {
            char size[4];
            if (get_input("Enter size of filename: ", size, sizeof(size)) == -1)
            {
                continue;
            }

            char *end;
            filename_size = strtol(size, &end, 10);

            if (*end != '\n' && *end != '\0')
            {
                printf("please enter a number only.\n");
                continue;
            }

            break;
        }

        ParsedCommand parsedCommand = parser(input_buffer, filename_size);
        printf("%s,%s,%s\n", parsedCommand.command, parsedCommand.filename, parsedCommand.content);
        response = is_command_valid(parsedCommand);
        if (response.error_count > 0)
        {
            set_message(message, response);
            printf("%s", message);
            continue;
        }

        result = send(sock, &filename_size, sizeof(filename_size), 0);
        if (result == -1)
        {
            perror("send (filename_size)");
            return 1;
        }

        result = send(sock, input_buffer, strlen(input_buffer), 0);
        if (result == -1)
        {
            perror("send");
            return 1;
        }

        memset(response_buffer, 0, sizeof(response_buffer));

        result = read(sock, response_buffer, BUFFER_SIZE);
        if (result == -1)
        {
            perror("read");
            return 1;
        }
        else if (result == 0)
        {
            printf("Server closed the connection.\n");
            break;
        }
        printf("%s", response_buffer);
    }

    close(sock);

    return 0;
}
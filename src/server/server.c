#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../../headers/types.h"
#include "../../headers/general/parser.h"
#include "../../headers/general/basic_validation.h"
#include "../../headers/general/functions.h"
#include "../../headers/server/validation.h"
#include "../../headers/server/file_manager.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int setup_server()
{
    int server_fd;
    struct sockaddr_in server_addr = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int accept_client(int server_fd)
{
    int client_fd;

    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd == -1)
    {
        perror("accept");
        return -1;
    }

    return client_fd;
}

int main()
{
    int server_fd, client_fd, result;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    ParsedCommand parsedCommand;
    Response response;

    server_fd = setup_server();

    if (server_fd == -1)
    {
        return 1;
    }

    printf("Server is running...\n");

    client_fd = accept_client(server_fd);

    if (client_fd == -1)
    {
        close(server_fd);
        return 1;
    }

    printf("Client connected!\n");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        memset(message, 0, sizeof(message));

        result = read(client_fd, buffer, sizeof(buffer));
        if (result == -1)
        {
            perror("read");
            return 1;
        }
        else if (result == 0)
        {
            strcpy(message, "Client disconnected.\n");
            if (send_message(client_fd, message) == -1)
            {
                return 1;
            }
            break;
        }

        if (buffer[0] == '\n')
        {
            strcpy(message, "Empty input. Please enter a valid command.\n");
            if (send_message(client_fd, message) == -1)
            {
                return 1;
            }
            continue;
        }

        parsedCommand = parser(buffer);
        response = is_command_valid(parsedCommand);
        if (handle_response(client_fd, message, response))
        {
            continue;
        }

        response = validation(parsedCommand);
        if (handle_response(client_fd, message, response))
        {
            continue;
        }

        response = file_manager(parsedCommand);
        if (handle_response(client_fd, message, response))
        {
            continue;
        }

        strcpy(message, response.success_message);
        if (send_message(client_fd, message) == 0)
        {
            return 1;
        }
    }

    printf("Closing connections...\n");
    close(client_fd);
    close(server_fd);

    return 0;
}
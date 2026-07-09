#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../../headers/types.h"
#include "../../headers/general/parser.h"
#include "../../headers/general/basic_validation.h"
#include "../../headers/server/validation.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void set_message(char *message, Response response)
{
    message = "Errors:\n";
    for (int i = 0; i < response.error_count; i++)
    {
        message = strcat(message, response.errors[i]);
        message = strcat(message, "\n");
    }
}

int send_message(int client_fd, char *message)
{
    int result = send(client_fd, message, strlen(message), 0);
    if (result == -1)
    {
        perror("send");
    }
    return result;
}

int main()
{
    int server_fd, client_fd, result;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hello from server!";
    ParsedCommand parsedCommand;
    Response response;

    // 1. יצירת socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    // 2. הגדרת השרת
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. bind
    result = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result == -1)
    {
        perror("bind");
        return 1;
    }

    // 4. listen
    result = listen(server_fd, 5);
    if (result == -1)
    {
        perror("listen");
        return 1;
    }

    printf("Server is running...\n");

    // 5. accept
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        perror("accept");
        return 1;
    }

    printf("Client connected!\n");

    while (1)
    {
        // איפוס הבאפר לפני כל קריאה חדשה כדי שלא יישארו שאריות מהקלט הקודם
        memset(buffer, 0, sizeof(buffer));

        // 6. recv
        result = read(client_fd, buffer, sizeof(buffer));
        if (result == -1)
        {
            perror("read");
            return 1;
        }
        else if (result == 0)
        {
            message = "Client disconnected.\n";
            if (send_message(client_fd, message) == -1)
            {
                return 1;
            }
            break;
        }

        if (buffer[0] == '\n')
        {
            message = "Empty input. Please enter a valid command.\n";
            if (send_message(client_fd, message) == -1)
            {
                return 1;
            }
            continue;
        }

        parsedCommand = parser(buffer);
        response = is_command_valid(parsedCommand);
        if (response.error_count > 0)
        {
            set_message(message, response);
            if (send_message(client_fd, "Validation errors:\n") == -1)
            {
                return 1;
            }
            continue;
        }

        response = validation(parsedCommand);
        if (response.error_count > 0)
        {
            set_message(message, response);
            if (send_message(client_fd, message) == -1)
            {
                return 1;
            }
            continue;
        }

        message = "Command received and validated successfully.\n";
        if (send_message(client_fd, message) == 0)
        {
            return 1;
        }
    }

    // 8. close
    printf("Closing connections...\n");
    close(client_fd);
    close(server_fd);

    return 0;
}
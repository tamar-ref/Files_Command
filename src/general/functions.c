#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../../headers/types.h"

void set_message(char *message, Response response)
{
    if (response.error_count == 1)
    {
        char temp[150];
        snprintf(temp, sizeof(temp),
                 "Error: %s\n",
                 response.errors[0]);

        strcat(message, temp);
    }
    else
    {
        for (int i = 0; i < response.error_count; i++)
        {
            char temp[150];
            snprintf(temp, sizeof(temp),
                     "Error %d: %s\n",
                     i + 1,
                     response.errors[i]);

            strcat(message, temp);
        }
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

int handle_response(int client_fd, char *message, Response response)
{
    printf("%d\n", response.error_count);
    if (response.error_count == 0)
    {
        return 0;
    }
    set_message(message, response);
    return send_message(client_fd, message);
}

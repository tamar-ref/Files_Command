#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <errno.h>

#include "parser.c"

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char input_buffer[BUFFER_SIZE] = {0};
    char response_buffer[BUFFER_SIZE] = {0};

    // 1. socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // כתובת localhost
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 2. connect
    int result = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result == -1)
    {
        perror("connect");
        return 1;
    }

    // 3. send
    while (1)
    {
        printf("Enter a message to send to server: ");
        fgets(input_buffer, sizeof(input_buffer), stdin);

        if (input_buffer[0] == '\n')
        {
            printf("Empty input. Please enter a valid command.\n");
            continue;
        }

        Response response = parser(input_buffer);
        printf("Result: '%s'\n", response.result);
        if (response.error_count > 0)
        {
            for (int i = 0; i < response.error_count; i++)
            {
                printf("Error %d: '%s'\n", i + 1, response.errors[i]);
            }
            continue;
        }

        result = send(sock, input_buffer, strlen(input_buffer), 0);
        if (result == -1)
        {
            perror("send");
            return 1;
        }

        // לנקות את מערך התשובה לפני שמקבלים מידע חדש
        memset(response_buffer, 0, sizeof(response_buffer));

        // 4. recv
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
        printf("Server says: %s\n", response_buffer);
    }

    // 5. close
    close(sock);

    return 0;
}
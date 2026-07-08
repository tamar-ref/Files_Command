#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

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
    int result = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
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
        result = read(client_fd, buffer, BUFFER_SIZE);
        if (result == -1)
        {
            perror("read");
            return 1;
        }
        else if (result == 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client says: %s\n", buffer);

        // 7. send response
        result = send(client_fd, "Done.", 5, 0);
        if (result == -1)
        {
            perror("send");
            return 1;
        }
    }

    // 8. close
    printf("Closing connections...\n");
    close(client_fd);
    close(server_fd);

    return 0;
}
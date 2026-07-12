#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../types.h"

void set_message(char *message, Response response);
int send_message(int client_fd, char *message);
int handle_response(int client_fd, char *message, Response response);

#endif
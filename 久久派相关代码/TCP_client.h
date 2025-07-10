#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "hx711.h"

#define MAX_MODULES 4

int tcp_client_init();
void *read_from_server(void *arg);
int send_weight_by_init(int client_fd, int index) ;
int send_weight_by_index(int client_fd, int index);
int send_all_weights(int client_fd);

#endif

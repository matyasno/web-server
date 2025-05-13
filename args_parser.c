//
// Created by matyas on 5/2/25.
//

#include "args_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void args_check(const int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: <ip> <port> <dir>\n");
        exit(-1);
    }
}

u_long get_host_order_ip(const char *host_ip) {
    return ntohl(inet_addr(host_ip));
}

int get_host_port(const char *host_port) {
    return atoi(host_port);
}

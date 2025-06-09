//
// Created by matyas on 5/2/25.
//

#include "args_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../defines.h"

void args_check(const int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: <ip> <port> <dir>\n");
        exit(-1);
    }
}

u_long get_host_order_ip(const char *host_ip) {
    return ntohl(inet_addr(host_ip));
}

int parse_port(const char *host_port) {
    int port;
    char *endptr;
    long port_long = strtol(host_port, &endptr, 10);

    if (*endptr != '\0' || port_long < 1 || port_long > 65535) {
        fprintf(stderr, "Invalid port number: %s\n", host_port);
        return ERROR_GENERIC;
    } else {
        port = (int)port_long;
    }
    return port;
}

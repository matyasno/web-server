//
// Created by matyas on 5/2/25.
//

#include "args_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void args_check(const int argc, char **argv) {
    if (argc != 5) {
        printf("Usage: <address family> <ip> <port> <dir>\n");
        exit(-1);
    }
}

u_long get_host_order_ip(const char *host_ip) {
    return ntohl(inet_addr(host_ip));
}

int get_host_port(const char *host_port) {
    return atoi(host_port);
}

int get_address_family(const char *address_family) {
    if (strcmp(address_family, "AF_INET") == 0) {
        return AF_INET;
    } else if (strcmp(address_family, "AF_INET6") == 0) {
        return AF_INET6;
    } else {
        fprintf(stderr, "Invalid address family: %s\n", address_family);
        exit(-1);
    }
}

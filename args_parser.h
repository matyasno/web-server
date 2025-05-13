//
// Created by matyas on 5/2/25.
//

#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H
#include <sys/types.h>

#endif //ARGS_PARSER_H

void args_check(int argc, char *argv[]);
u_long get_host_order_ip(const char *host_ip);
int get_host_port(const char *host_port);
int get_address_family(const char *address_family);

#include <unistd.h>

#include "server.h"

#include <stdlib.h>
#include <stdio.h>

#include "args_parser.h"

#ifdef _WIN32
    #define SHUT_RDWR SD_BOTH
#endif

int main(int argc, char **argv) {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        exit(1);
    }
#endif
    args_check(argc, argv);
    struct Server server = server_constructor(SOCK_STREAM, 0, argv[1], argv[2], 5);
    start_http_server(&server, argv[3]);
    return 0;
}

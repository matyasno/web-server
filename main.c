#include "server/core/server.h"
#include "server/arguments/args_parser.h"

int main(int argc, char **argv) {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        exit(1);
    }
#endif
    int protocol = 0;
    int port = 0;
    int backlog = 0;
    char server_interface[50] = "";
    char web_directory[50] = "";
    parse_args(argc, argv, &port, server_interface, &protocol, &backlog, web_directory);
    struct Server server = create_server(SOCK_STREAM, protocol, argv[1], argv[2], 5);
    start_http_server(&server, argv[3]);
    return 0;

#ifdef _WIN32
    WSACleanup();
#endif

}

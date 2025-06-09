#include "server/core/server.h"
#include "server/arguments/args_parser.h"
#include "server/config/config_parser.h"

int main(int argc, char **argv) {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        exit(1);
    }
#endif
    server_config cfg;
    parse_args(argc, argv, &cfg);
    struct Server server = create_server(SOCK_STREAM, cfg.protocol, cfg.interface, cfg.port, cfg.backlog);
    start_http_server(&server, cfg.web_root);
    return 0;

#ifdef _WIN32
    WSACleanup();
#endif

}

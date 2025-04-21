#include "Server.h"
#include <unistd.h>
#include <string.h>

#include "servUtil.h"

#ifdef _WIN32
    #define SHUT_RDWR SD_BOTH
#endif

void launch(struct Server *server) {
    printf("=== SERVER LAUNCHED ===\n");

    while (1) {
        char request[1000] = {0};
        const char* rootDir="../testWeb/";

        printf("Waiting for connection...\n");
        const int clientHandle=getClientHandle(server);

        if (getClientRequest(clientHandle,request, sizeof(request)-1) != 0) {
            continue;
        }

        const char* fullPath = getFilePath(request, rootDir);
        const char* HTMLContent = getHTMLContent(fullPath);
        const char* response = HTMLContent ? buildResponse(HTMLContent) : buildNotFoundResponse();

        send(clientHandle, response, strlen(response), 0);

        printf("\nResponse sent, closing connection.\n");

        free((void*)fullPath);
        free((void*)HTMLContent);
        free((void*)response);

        shutdown(clientHandle, SHUT_RDWR);
        close(clientHandle);
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        exit(1);
    }
#endif

    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 10, launch);
    server.launch(&server);
    return 0;
}
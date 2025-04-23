//
// Created by matyas on 4/20/25.
//

#ifndef SERVUTIL_H
#define SERVUTIL_H

#endif //SERVUTIL_H

#include "Server.h"

// net shit
int getClientHandle(struct Server *server);
int getClientRequest(int clientHandle, char* buff, size_t buffSize);

// parsing/building client requests
int getHTMLContent(const char* request, const char* rootDir, char* buff, size_t buffSize);
int getRequestedFile(const char* request, char* buff, size_t buffSize);
int getFilePath(const char* request, const char* currentWorkingDir, char* buff, size_t buffSize);
int buildResponse(const char* HTMLContent, char* buff, size_t buffSize);
int  buildNotFoundResponse(char* buff, size_t buffSize);

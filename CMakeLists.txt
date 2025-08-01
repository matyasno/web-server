cmake_minimum_required(VERSION 3.30)
project(web-server)

set(CMAKE_CXX_STANDARD 20)

add_executable(web-server main.c
        server/core/server.c
        server/core/server.h
        server/request/request_handler.c
        server/request/request_handler.h
        server/utils/net_utils.c
        server/utils/net_utils.h
        server/defines.h
        server/request/request_parser.h
        server/request/request_parser.c
        server/utils/file_utils.h
        server/utils/file_utils.c
        server/response/response_handler.c
        server/response/response_handler.h
        server/arguments/args.c
        server/arguments/args.h
        server/logger/logger.c
        server/logger/logger.h
        server/logger/log_defines.h
        server/config/config.c
        server/config/config.h
)

IF (WIN32)
    target_link_libraries(web-server ws2_32)
ELSE ()
    target_link_libraries(web-server)
ENDIF ()
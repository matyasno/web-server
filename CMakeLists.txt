cmake_minimum_required(VERSION 3.30)
project(web-server)

set(CMAKE_CXX_STANDARD 20)

add_executable(web-server main.c
        server.c
        server.h
        request_handler.c
        request_handler.h
        net_utils.c
        net_utils.h
        defines.h
        request_parser.h
        request_parser.c
        file_utils.h
        file_utils.c
        response.c
        response.h
        args_parser.c
        args_parser.h)

IF (WIN32)
    target_link_libraries(web-server ws2_32)
ELSE ()
    target_link_libraries(web-server)
ENDIF ()
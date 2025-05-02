cmake_minimum_required(VERSION 3.30)
project(web-server)

set(CMAKE_CXX_STANDARD 20)

add_executable(web-server test.c
        server.c
        server.h
        server_utils.c
        server_utils.h
        net_utils.c
        net_utils.h
        defines.h)

IF (WIN32)
    target_link_libraries(web-server ws2_32)
ELSE ()
    target_link_libraries(web-server)
ENDIF ()
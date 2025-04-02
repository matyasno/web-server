cmake_minimum_required(VERSION 3.30)
project(Cserver)

set(CMAKE_CXX_STANDARD 20)

add_executable(Cserver test.c
        Server.c
        Server.h)

target_link_libraries(Cserver ws2_32)

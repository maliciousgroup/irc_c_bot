cmake_minimum_required(VERSION 3.12)
project(irc C)

set(CMAKE_C_STANDARD 99)

add_executable(irc main.c socket.c socket.h ssl.c ssl.h irc.c irc.h random.c random.h)

target_link_libraries(irc ssl crypto)
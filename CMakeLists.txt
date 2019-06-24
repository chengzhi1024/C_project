cmake_minimum_required(VERSION 3.14)
project(C_project C)

set(CMAKE_C_STANDARD 11)

add_executable(C_project main.c sql.h sql.c Server.c Server.h " Client.c" " Client.h" sql_test.c)
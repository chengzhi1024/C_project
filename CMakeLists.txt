cmake_minimum_required(VERSION 3.14)
project(C_project C)

set(CMAKE_C_STANDARD 11)



add_executable(C_project main.c db_mysql/db_interface.h db_mysql/db_interface.c business.c business.h " Client.c" " Client.h" db_mysql/db_mysql_test.c)


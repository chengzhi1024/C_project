cmake_minimum_required(VERSION 3.14)
project(C_project C)

set(CMAKE_C_STANDARD 11)

include_directories(Cjson/)
include_directories(db_mysql/)
include_directories(socke/)

add_executable(C_project main.c business.c Cjson/cJSON.c Cjson/cJSON.h db_mysql/db_mysql.c socke/socket_server.c)
target_link_libraries(C_project libmysqlclient.so)
cmake_minimum_required(VERSION 3.14)
project(C_project C)

set(CMAKE_C_STANDARD 11)
include_directories(C:\\Program Files\\MySQL\\MySQL Server 8.0\\include)
include_directories(Cjson/)
include_directories(db_mysql/)

add_executable(C_project main.c business.c Cjson/cJSON.c Cjson/cJSON.h temp.c db_mysql/db_mysql.c)
//
// Created by yang on 2019/6/24.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sql.h>

static void print_sql_error(MYSQL *sql_conn, const char *msg)
{
    printf("error");
}

static int db_exec_sql(MYSQL *sql_conn, const char *sql)
{
    printf("db_exec_sql");
}

int create_database(void *pvDBHandle, const char *pcszDBName)
{
    printf("create_database");
}

int create_table(void *pvDBHandle, const char *pcszTable, const char *pcszSqlCommand)
{
    printf("create_table");
}

const char* db_get_error(void *ppvDBHandle)
{
    printf("db_get_error");
}

int db_init(void **ppvDBHandle)
{
    printf("db_init");
}

int db_connect(void *pvDBHandle, const char *pcszHost, short nPort, const char *pcszUser, const char *pcszPassword, const char *pcszDBName)
{
    printf("db_connect");
}

int db_insert(void *pvDBHandle, const char *pSqlCommand)
{
    printf("db_insert");
}

int db_delete(void *pvDBHandle, const char *pSqlCommand)
{
    printf("db_delete");
}

int db_update(void *pvDBHandle, const char *pSqlCommand)
{
    printf("db_update");
}

int db_query(void *pvDBHandle, const char *pSqlCommand, db_fetch_callback_t cb)
{
    printf("db_query");
}

void db_uninit(void *pvDBHandle)
{
    printf("db_uninit");
}




//
// Created by yang on 2019/6/24.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sql.h>

static void print_sql_error(MYSQL *sql_conn, const char *msg)
{
    if(msg)
        printf("%s: %s\n", msg, mysql_error(sql_conn));
    else
        printf("%s\n", mysql_error(sql_conn));
}

static int db_exec_sql(MYSQL *sql_conn, const char *sql)
{
    return mysql_real_query(sql_conn, sql, strlen(sql));
}

int create_database(void *pvDBHandle, const char *pcszDBName)
{
    MYSQL *pSqlConn = pvDBHandle;
    char sql[SQL_COMMAND_MAX_SIZE] = {0};
    if (NULL == pcszDBName) return -1;

    snprintf(sql, sizeof(sql), "use %s", pcszDBName);
    if (db_exec_sql(pSqlConn, sql)) {
        snprintf(sql, sizeof(sql), "create database %s;", pcszDBName);
        db_exec_sql(pSqlConn, sql);
        print_sql_error(pSqlConn, NULL);

        snprintf(sql, sizeof(sql), "use %s;", pcszDBName);
        db_exec_sql(pSqlConn, sql);
        print_sql_error(pSqlConn, NULL);
    }
    else {
        snprintf(sql, sizeof(sql), "use %s;", pcszDBName);
        db_exec_sql(pSqlConn, sql);
        print_sql_error(pSqlConn, NULL);
    }

    return 0;
}

int create_table(void *pvDBHandle, const char *pcszTable, const char *pcszSqlCommand)
{
    int iRet = -1;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int num_rows;
    MYSQL *pSqlConn = pvDBHandle;
    char sql[SQL_COMMAND_MAX_SIZE] = {0};

    snprintf(sql, sizeof(sql), "show %s;", pcszTable);
    db_exec_sql(pSqlConn, sql);
    res = mysql_store_result(pSqlConn);
    if (!res) return mysql_errno(pSqlConn);

    num_rows = mysql_num_rows(res);
    if (0 == num_rows) {
        iRet = db_exec_sql(pSqlConn, pcszSqlCommand);
    }
    mysql_free_result(res);
    return iRet;
}

const char* db_get_error(void *ppvDBHandle)
{
    return mysql_error((MYSQL *)ppvDBHandle);
}

int db_init(void **ppvDBHandle)
{
    if (NULL == ppvDBHandle) return -1;
    *ppvDBHandle = mysql_init(NULL);
    if (NULL == *ppvDBHandle) return -1;
    else return 0;
}

int db_connect(void *pvDBHandle, const char *pcszHost, short nPort, const char *pcszUser, const char *pcszPassword, const char *pcszDBName)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pcszHost || NULL == pcszUser || NULL == pcszPassword) return -1;
    if (NULL == mysql_real_connect(pSqlConn, pcszHost, pcszUser, pcszPassword, pcszDBName, nPort, NULL, 0)) {
        return mysql_errno(pvDBHandle);
    }
    return mysql_set_character_set(pSqlConn, "utf8");
}

int db_insert(void *pvDBHandle, const char *pSqlCommand)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pSqlCommand) {
        return -1;
    }
    return db_exec_sql(pSqlConn, pSqlCommand);
}

int db_delete(void *pvDBHandle, const char *pSqlCommand)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pSqlCommand) {
        return -1;
    }
    return db_exec_sql(pSqlConn, pSqlCommand);
}

int db_update(void *pvDBHandle, const char *pSqlCommand)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pSqlCommand) {
        return -1;
    }
    return db_exec_sql(pSqlConn, pSqlCommand);
}

int db_query(void *pvDBHandle, const char *pSqlCommand, db_fetch_callback_t cb)
{
    int iRet;
    int iRow, iCol, iCurRow = 0;
    char **ppRow;
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pSqlCommand) {
        return -1;
    }
    if (iRet = db_exec_sql(pSqlConn, pSqlCommand)) return iRet;

    MYSQL_RES *result = mysql_store_result(pSqlConn);
    if (NULL == result) return mysql_errno(pSqlConn);

    iRow = mysql_num_rows(result);
    iCol = mysql_num_fields(result);
    while(ppRow = mysql_fetch_row(result)) {
        if (ppRow) {
            if (cb(ppRow, ++iCurRow, iCol))
                break;
        }
    }

    mysql_free_result(result);

    return 0;

}

void db_uninit(void *pvDBHandle)
{
    if (NULL != pvDBHandle) mysql_close(pvDBHandle);
}



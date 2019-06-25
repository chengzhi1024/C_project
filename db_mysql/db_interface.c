//
// Created by yang on 2019/6/24.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "db_interface.h"
#include <mysql/mysql.h>

/**
 * @brief 打印错误信息
 * @param sql_conn 连接数据库的句柄
 * @param msg 返回的消息
 */
static void print_sql_error(MYSQL *sql_conn, const char *msg)
{
    if(msg)
        printf("%s: %s\n", msg, mysql_error(sql_conn));
    else
        printf("%s\n", mysql_error(sql_conn));
}

/**
 *
 * @brief 数据库命令执行函数接口
 * @param sql_conn  数据库连接的句柄
 * @param sql sql命令语句的地址
 * @return
 */
static int db_exec_sql(MYSQL *sql_conn, const char *sql)
{
    return mysql_real_query(sql_conn, sql, strlen(sql));   //执行由query指向的SQL查询
}

/**
 * @brief 建立数据库，如果存在则使用这个数据库
 * @param pvDBHandle  数据库句柄
 * @param pcszDBName  需要建的库名
 * @return 成功返回0，失败返回1
 */
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

/**
 *
 * @brief 建表
 * @param pvDBHandle 数据库句柄
 * @param pcszTable 表名
 * @param pcszSqlCommand  sql命令字符串
 * @return 成功返回0，失败返回1
 */
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
/**
 * @brief 返回上一个 MySQL 操作产生的文本错误信息。
   本函数返回上一个 MySQL 函数的错误文本，如果没有出错则返回 ''（空字符串）。
 * @param ppvDBHandle
 * @return 返回一个字符串
 */
const char* db_get_error(void *ppvDBHandle)
{
    return mysql_error((MYSQL *)ppvDBHandle);
}

/**
 * @brief 初始化数据库，拿到一个操作句柄
 * @param ppvDBHandle
 * @return 成功返回0，失败返回1
 */
int db_init(void **ppvDBHandle)
{
    if (NULL == ppvDBHandle) return -1;
    *ppvDBHandle = mysql_init(NULL);
    if (NULL == *ppvDBHandle) return -1;
    else return 0;
}

/**
 * @brief 连接数据库
 * @param pvDBHandle  数据库句柄
 * @param pcszHost  主机号
 * @param nPort 端口号
 * @param pcszUser  主机用户名
 * @param pcszPassword  用户密码
 * @param pcszDBName  数据库名，如果不存在则为NULL
 * @return 成功返回0，失败返回1
 */
int db_connect(void *pvDBHandle, const char *pcszHost, short nPort, const char *pcszUser, const char *pcszPassword, const char *pcszDBName)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pcszHost || NULL == pcszUser || NULL == pcszPassword) return -1;
    if (NULL == mysql_real_connect(pSqlConn, pcszHost, pcszUser, pcszPassword, pcszDBName, nPort, NULL, 0)) {
        return mysql_errno(pvDBHandle);
    }
    return mysql_set_character_set(pSqlConn, "utf8");
}
/**
 * @brief 插入数据库一条信息的接口
 * @param pvDBHandle  数据库句柄
 * @param pSqlCommand  sql命令字符串
 * @return 成功返回0，失败返回1
 */
int db_insert(void *pvDBHandle, const char *pSqlCommand)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pSqlCommand) {
        return -1;
    }
    return db_exec_sql(pSqlConn, pSqlCommand);
}

/**
 * @brief 删除数据库信息 接口
 * @param pvDBHandle  数据库句柄
 * @param pSqlCommand  sql命令字符串
 * @return 成功返回0，失败返回1
 */
int db_delete(void *pvDBHandle, const char *pSqlCommand)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pSqlCommand) {
        return -1;
    }
    return db_exec_sql(pSqlConn, pSqlCommand);
}

/**
 * @brief 更新数据库信息接口
 * @param pvDBHandle 数据库句柄
 * @param pSqlCommand  sql命令字符串
 * @return 成功返回0，失败返回1
 */
int db_update(void *pvDBHandle, const char *pSqlCommand)
{
    MYSQL *pSqlConn = pvDBHandle;
    if (NULL == pvDBHandle || NULL == pSqlCommand) {
        return -1;
    }
    return db_exec_sql(pSqlConn, pSqlCommand);
}
/**
 * @brief 查询数据库中的某条信息
 * @param pvDBHandle  数据库句柄
 * @param pSqlCommand sql命令字符串
 * @param cb  回调函数，处理其他的业务，然后返还给本函数需要的值
 * @param pvData  回调函数的一个参数，接收回调函数传回来的信息
 * @return 成功返回0，失败返回1
 */
int db_query(void *pvDBHandle, const char *pSqlCommand, db_fetch_callback_t cb, void *pvData)
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
            if (cb(ppRow, ++iCurRow, iCol, pvData))
                break;
        }
    }

    mysql_free_result(result);

    return 0;

}
/**
 * @brief 关闭数据库
 * @param pvDBHandle  数据库句柄
 */

void db_uninit(void *pvDBHandle)
{
    if (NULL != pvDBHandle) mysql_close(pvDBHandle);
}




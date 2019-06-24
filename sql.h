//
// Created by yang on 2019/6/24.
//
#pragma once
#ifndef _SQL_WRAP_H
#define _SQL_WRAP_H

//#include <mysql.h>

#define SQL_COMMAND_MAX_SIZE 4096

#ifdef __cplusplus
extern "C" {
#endif

#if 1
#define DEBUG(format, ...) printf(format",Line:%d, Function:%s, File:%s\n", ##__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__);
#else
#define DEBUG(format, ...)
#endif
typedef int (*db_fetch_callback_t)(char **ppcArgs, int iRow, int iCol);

int db_init(void **ppvDBHandle);
int create_database(void *pvDBHandle, const char *pcszDBName);
int create_table(void *pvDBHandle, const char *pcszTable, const char *pcszSqlCommand);
int db_connect(void *pvDBHandle, const char *c_pszHost, short nPort, const char *c_pszUser, const char *c_pszPassword, const char *c_szDBName);
int db_insert(void *pvDBHandle, const char *pSqlCommand);
int db_delete(void *pvDBHandle, const char *pSqlCommand);
int db_update(void *pvDBHandle, const char *pSqlCommand);
int db_query(void *pvDBHandle, const char *pSqlCommand, db_fetch_callback_t cb);
void db_uninit(void *pvDBHandle);
const char* db_get_error(void *ppvDBHandle);
#ifdef __cplusplus
}
#endif
#endif


//
//
//#include <stdio.h>
//#include "db_mysql/db_mysql.h"
//
//int main() {
//    void *pvDBHandle = NULL;
//    void **ppvDBHandle = NULL;
//    char *pcszDBName = NULL;
//    char *pcszTable = NULL;
//    char *c_pszHost = NULL;
//    char *pSqlCommand = NULL;
//    char *pcszSqlCommand = NULL;
//    char *c_pszUser = NULL;
//    char *c_pszPassword = NULL;
//    char *c_szDBName = NULL;
//    short nPort = 0;
////    db_fetch_callback_t cb = 0;
//
//    db_init(ppvDBHandle);
//    create_database(pvDBHandle, pcszDBName);
//    create_table(pvDBHandle, pcszTable, pcszSqlCommand);
//    db_connect(pvDBHandle, c_pszHost, nPort, c_pszUser, c_pszPassword, c_szDBName);
//    db_insert(pvDBHandle, pSqlCommand);
//    db_delete(pvDBHandle, pSqlCommand);
//    db_update(pvDBHandle, pSqlCommand);
////    db_query(pvDBHandle, pSqlCommand, db_fetch_callback_t cb);
//    db_uninit(pvDBHandle);
//    db_get_error(ppvDBHandle);
//    printf("Hello, World!\n");
//    return 0;
//}



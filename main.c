

#include <stdio.h>
#include "db_mysql/db_mysql.h"

int main() {



    int iRet;
    void *pvDBHandle  = NULL;
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];
    char *psSqlCommand = NULL;

    iRet = db_init(&pvDBHandle);
    if (iRet) return iRet;
    printf("db_init:%s\n", db_get_error(pvDBHandle));

    do {

        iRet = db_connect(pvDBHandle, "10.2.8.120", 3306, "root", "123456dsa", "library system");
        printf("db_connect iRet:%d\n", iRet);
        printf("%s\n", db_get_error(pvDBHandle));
        if (iRet) break;
    }while (0);
    return 0;
//        void *pvDBHandle=NULL;
//    char *pcszHost=NULL;
//    db_connect(pvDBHandle, const char *pcszHost, short nPort, const char *pcszUser, const char *pcszPassword, const char *pcszDBName)
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
    return 0;
}

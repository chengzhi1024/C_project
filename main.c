

#include <stdio.h>
#include "db_mysql/db_interface.h"
#include "business.h"


int main(int argc, char *argv[])
    {
        int iRet;
        /* 获取数据库操作句柄 */
        void **ppvDBHandle  = get_handle();

        /* 初始化数据库 */
        iRet = db_init(ppvDBHandle);
        if (iRet) return iRet;
        printf("db_init:%s\n", db_get_error(*ppvDBHandle));

        /* 连接数据库 */
        iRet = db_connect(*ppvDBHandle, "localhost", 3306, "root", "123456", "library_system");
        printf("db_connect iRet:%d\n", iRet);
        printf("%s\n", db_get_error(*ppvDBHandle));
        if (iRet) return iRet;

        // 初始化socket服务，进入socket事件循环
        recv_handle_t cb = exec_business;
        int iSockFd = socket_init(9999);
        socket_accept(iSockFd, cb);

        /* 退出系统 */
        socket_uninit(iSockFd);
        db_uninit(*ppvDBHandle);

        return 0;
    }


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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "business.h"
#include "Cjson/cJSON.h"
#include "db_mysql.h"

struct request {
    int messageId;
    char adminId[48];
    char adminPasswd[48];
    char adminName[48];
} request;

int main() {

    int iRet;
    /* 获取数据库操作句柄 */
    void **ppvDBHandle  = get_handle();

    /* 初始化数据库 */
    iRet = db_init(ppvDBHandle);
    if (iRet) return iRet;
    printf("db_init:%s\n", db_get_error(*ppvDBHandle));

    /* 连接数据库 */
    iRet = db_connect(*ppvDBHandle, "10.2.8.114", 3306, "root", "123456dsa", "library_system");
//    printf("db_connect iRet:%d\n", iRet);
//    printf("%s\n", db_get_error(*ppvDBHandle));
    if (iRet) return iRet;

//    // 初始化socket服务，进入socket事件循环
//    recv_handle_t cb = exec_business;
//    int iSockFd = socket_init(9999);
//    socket_accept(iSockFd, cb);

    //创建一个cjson结构体指针
    cJSON *root = cJSON_CreateObject();
    //添加键值队
    cJSON_AddNumberToObject(root, "messageId", 1002);
    cJSON_AddStringToObject(root, "adminId", "3");
    cJSON_AddStringToObject(root, "adminPasswd", "456");
    cJSON_AddStringToObject(root, "adminName", "yang");
    char *out = cJSON_PrintUnformatted(root);

    char *pRequest = out;
    int iReqLen=1024;
    char pResponse[1024];
    int iResLen=1024;

    printf("pRequest:%s\n", pRequest);
    exec_business(pRequest, iReqLen, pResponse, iResLen);
    printf("pResponse:%s\n", pResponse);

    /* 退出系统 */
//    socket_uninit(iSockFd);
    db_uninit(*ppvDBHandle);
    return 0;
}

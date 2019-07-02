#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "business.h"
#include "Cjson/cJSON.h"
#include "db_mysql.h"
#include "socket_server.h"

int main() {
    int iRet;
    /* 获取数据库操作句柄 */
    void **ppvDBHandle = get_handle();
    /* 初始化数据库 */
    iRet = db_init(ppvDBHandle);
    if (iRet) return iRet;
    printf("db_init:%s\n", db_get_error(*ppvDBHandle));
    /* 连接数据库 */
    iRet = db_connect(*ppvDBHandle, "10.2.8.85", 3306, "root", "123456dsa", "library_system");
//    printf("db_connect iRet:%d\n", iRet);
//    printf("%s\n", db_get_error(*ppvDBHandle));
    if (iRet) return iRet;
#if 1
    // 初始化socket服务，进入socket事件循环
    recv_handle_t cb = exec_business;
    int iSockFd = socket_init(9999);
    socket_accept(iSockFd, cb);

    /* 退出系统 */
    socket_uninit(iSockFd);
    db_uninit(*ppvDBHandle);
#else
    //创建一个cjson结构体指针
    cJSON *root = cJSON_CreateObject();

//    //添加键值队
//    cJSON_AddNumberToObject(root, "messageId", 1002);
//    cJSON_AddStringToObject(root, "adminId", "1");
//    cJSON_AddStringToObject(root, "adminPasswd", "123");
//    cJSON_AddStringToObject(root, "adminName", "yang");

////    cJSON_AddNumberToObject(root, "messageId", 1003);
////    cJSON_AddStringToObject(root, "adminId", "1");
////    cJSON_AddStringToObject(root, "adminOldPasswd", "456");
////    cJSON_AddStringToObject(root, "adminNewPasswd", "789");
////
////    cJSON_AddNumberToObject(root, "messageId", 1004);
////    cJSON_AddStringToObject(root, "adminId", "0001");
//
////    cJSON_AddNumberToObject(root, "messageId", 5001);
////    cJSON_AddStringToObject(root, "userNum", "4");
////    cJSON_AddStringToObject(root, "userName", "yang");
////    cJSON_AddStringToObject(root, "userSex", "");
////    cJSON_AddStringToObject(root, "userRemark", "");

//        cJSON_AddNumberToObject(root, "messageId", 3002);
//        cJSON_AddStringToObject(root, "userNum", "1");

//    cJSON_AddNumberToObject(root, "messageId", 2002);
//    cJSON_AddStringToObject(root, "bookId", "1234");
//    cJSON_AddStringToObject(root, "bookName", "yang");
//    cJSON_AddStringToObject(root, "bookAuthor", "yangv");
//    cJSON_AddNumberToObject(root, "bookAddNumber", 50);
////    cJSON_AddStringToObject(root, "bookRemain", "");
////    cJSON_AddStringToObject(root, "bookTimes", "");
//    cJSON_AddStringToObject(root, "bookCategory", "数学");
//    cJSON_AddStringToObject(root, "bookPublisher", "触犯");
//    cJSON_AddStringToObject(root, "bookPublicationDate", "2019-06-25");


    cJSON_AddNumberToObject(root, "messageId", 4001);
    cJSON_AddStringToObject(root, "userNum", "2");
    cJSON_AddStringToObject(root, "bookId", "2001");

//    cJSON_AddNumberToObject(root, "messageId", 2001);
//    cJSON_AddNumberToObject(root, "start", 20);
//    cJSON_AddNumberToObject(root, "end", 30);
////
//    cJSON_AddNumberToObject(root, "messageId", 2003);
//    cJSON_AddStringToObject(root, "bookSearch", "yang");
//    cJSON_AddNumberToObject(root, "type", 1);

    char *out = cJSON_PrintUnformatted(root);

    char *pRequest = out;
    int iReqLen = 10240;
    char pResponse[10240];
    int iResLen = 10240;

    printf("pRequest:%s\n", pRequest);
    exec_business(pRequest, iReqLen, pResponse, iResLen);
    printf("pResponse:%s\n", pResponse);
#endif
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "business.h"
#include "db_mysql.h"
#include "cJSON.h"


/** 登录查询返回函数 */
static int admin_login_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 0; i < iCol; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}

//登录函数
static void admin_login_in(cJSON *root, char *pResponse, int iResLen) {

    admin_info_t stAdmin;                   //创建一个管理员结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区

    int iCbRet = -1;                        //定义一个返回值
    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给管理员结构体
    if (cJSON_HasObjectItem(root, "adminId")) {
        //将json中adminId打印（复制）到stAdmin.sAdminId
        int flag = snprintf(stAdmin.sAdminId, sizeof(stAdmin.sAdminId), "%s",
                            cJSON_GetObjectItem(root, "adminId")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -5;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
        }
    } else {
        stAdminResponse.iErrorCode = -5;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
    }
    if (cJSON_HasObjectItem(root, "adminPasswd")) {
        int flag = snprintf(stAdmin.sAdminPasswd, sizeof(stAdmin.sAdminPasswd), "%s",
                            cJSON_GetObjectItem(root, "adminPasswd")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -6;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminPasswd");
        }
    } else {
        stAdminResponse.iErrorCode = -6;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminPasswd");
    }
//    if (cJSON_HasObjectItem(root, "adminName")) {
//        int flag = snprintf(stAdmin.sAdminName, sizeof(stAdmin.sAdminName), "%s",
//                            cJSON_GetObjectItem(root, "adminName")->valuestring);
//        if (flag < 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        } else if (flag == 0) {
//            stAdminResponse.iErrorCode = -7;
//            strcpy(stAdminResponse.sErrorDetail, "NO_adminName");
//        }
//    } else {
//        stAdminResponse.iErrorCode = -7;
//        strcpy(stAdminResponse.sErrorDetail, "NO_adminName");
//    }

    printf("sAdminId:%s,sAdminPasswd:%s,sAdminName:%s\n",
           stAdmin.sAdminId, stAdmin.sAdminPasswd, stAdmin.sAdminName);

    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_admin where adminId=\"%s\";",
                 stAdmin.sAdminId) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /* 查询数据库是否存在adminId对应的管理员 */
    db_query(*ppvDBHandle, szSqlBuffer, admin_login_in_callback, &iCbRet);
    //iCbRet为0，即adminId对应的管理员存在 否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }

    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        iCbRet = -1;
        //继续生成查询用户的其他信息的命令
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                     "select * from lib_admin where adminId=\"%s\" and adminPasswd=\"%s\";",
                     stAdmin.sAdminId, stAdmin.sAdminPasswd) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        //继续查询用户的其他信息是否存在 用户名和密码是否正确
        db_query(*ppvDBHandle, szSqlBuffer, admin_login_in_callback, (void *) &iCbRet);
        //再次判断用户名和密码是否正确，否则返回admin_response_t
        if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = -1;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_PASSWD_ERROR");
        }
        if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_OK");
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 1001);
    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
}

/** 注册查询返回函数 */
static int admin_register_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的adminid的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 0; i < 1; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}

//注册函数
static void admin_register_in(cJSON *root, char *pResponse, int iResLen) {

    admin_info_t stAdmin;                   //创建一个管理员结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区

    int iCbRet = -1;                        //定义一个返回值
    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给管理员结构体
    if (cJSON_HasObjectItem(root, "adminId")) {
        //将json中adminId打印（复制）到stAdmin.sAdminId
        int flag = snprintf(stAdmin.sAdminId, sizeof(stAdmin.sAdminId), "%s",
                            cJSON_GetObjectItem(root, "adminId")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -5;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
        }
    } else {
        stAdminResponse.iErrorCode = -5;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
    }
    if (cJSON_HasObjectItem(root, "adminPasswd")) {
        int flag = snprintf(stAdmin.sAdminPasswd, sizeof(stAdmin.sAdminPasswd), "%s",
                            cJSON_GetObjectItem(root, "adminPasswd")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -6;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminPasswd");
        }
    } else {
        stAdminResponse.iErrorCode = -6;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminPasswd");
    }
    if (cJSON_HasObjectItem(root, "adminName")) {
        int flag = snprintf(stAdmin.sAdminName, sizeof(stAdmin.sAdminName), "%s",
                            cJSON_GetObjectItem(root, "adminName")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -7;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminName");
        }
    } else {
        stAdminResponse.iErrorCode = -7;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminName");
    }

    printf("sAdminId:%s,sAdminPasswd:%s,sAdminName:%s\n",
           stAdmin.sAdminId, stAdmin.sAdminPasswd, stAdmin.sAdminName);


    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_admin where adminId=\"%s\";",
                 stAdmin.sAdminId) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /* 查询数据库是否存在adminId对应的管理员 */
    db_query(*ppvDBHandle, szSqlBuffer, admin_register_in_callback, &iCbRet);
    //iCbRet为0即adminId对应的管理员是存在 返回admin_response_t错误信息 否则添加一个管理员
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -4;
        strcpy(stAdminResponse.sErrorDetail, "ADMIN_ALREADY_EXIST");
    }
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        //将查询信息打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "insert into lib_admin values ('%s','%s','%s');",
                     stAdmin.sAdminId, stAdmin.sAdminPasswd, stAdmin.sAdminName) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        /* 插入管理员 */
        iCbRet = db_insert(*ppvDBHandle, szSqlBuffer);
        //如果iCbRet为0，则插入成功 否则返回admin_response_t错误信息
        if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = -8;
            strcpy(stAdminResponse.sErrorDetail, "INSERT_FAILURED");
        }
        //如果iCbRet为0，则插入成功
        if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0k");
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 1002);
    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
}

/** 密码修改查询返回函数 */
static int admin_modifyPassword_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的adminid的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 0; i < 2; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}

//管理员密码修改
static void admin_modifyPassword_in(cJSON *root, char *pResponse, int iResLen) {

    admin_modifypassword_t stAdmin;         //创建一个管理员修改密码结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区

    int iCbRet = -1;                        //定义一个返回值
    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给管理员修改密码结构体
    if (cJSON_HasObjectItem(root, "adminId")) {
        //将json中adminId打印（复制）到stAdmin.sAdminId
        int flag = snprintf(stAdmin.sAdminId, sizeof(stAdmin.sAdminId), "%s",
                            cJSON_GetObjectItem(root, "adminId")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -5;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
        }
    } else {
        stAdminResponse.iErrorCode = -5;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
    }
    if (cJSON_HasObjectItem(root, "adminOldPasswd")) {
        int flag = snprintf(stAdmin.sAdminOldPasswd, sizeof(stAdmin.sAdminOldPasswd), "%s",
                            cJSON_GetObjectItem(root, "adminOldPasswd")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -9;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminOldPasswd");
        }
    } else {
        stAdminResponse.iErrorCode = -9;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminOldPasswd");
    }
    if (cJSON_HasObjectItem(root, "adminNewPasswd")) {
        int flag = snprintf(stAdmin.sAdminNewPasswd, sizeof(stAdmin.sAdminNewPasswd), "%s",
                            cJSON_GetObjectItem(root, "adminNewPasswd")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -10;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminNewPasswd");
        }
    } else {
        stAdminResponse.iErrorCode = -10;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminNewPasswd");
    }

    printf("sAdminId:%s,sAdminOldPasswd:%s,sAdminNewPasswd:%s\n",
           stAdmin.sAdminId, stAdmin.sAdminOldPasswd, stAdmin.sAdminNewPasswd);


    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                 "select * from lib_admin where adminId=\"%s\" and adminPasswd=\"%s\";",
                 stAdmin.sAdminId, stAdmin.sAdminOldPasswd) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /* 查询数据库是否存在adminId对应的管理员 */
    db_query(*ppvDBHandle, szSqlBuffer, admin_modifyPassword_in_callback, &iCbRet);
    //iCbRet为0即adminId对应的管理员是存在  否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {

        //将修改密码命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                     "update lib_admin set adminPasswd = \"%s\" where adminId = \"%s\";",
                     stAdmin.sAdminNewPasswd, stAdmin.sAdminId) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        /* 修改管理员密码 */
        iCbRet = db_update(*ppvDBHandle, szSqlBuffer);
        //如果iCbRet为0，则插入成功 否则返回admin_response_t错误信息
        if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = -11;
            strcpy(stAdminResponse.sErrorDetail, "MODIFYPASSWORD_FAILURED");
        }
        //如果iCbRet为0，则插入成功
        if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0k");
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 1003);
    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
}

/** 管理员注销返回函数 */
static int admin_logout_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的adminid的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 0; i < iCol; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}

//管理员密码修改
static void admin_logout_in(cJSON *root, char *pResponse, int iResLen) {

    char sAdminId[24];                      //创建一个管理员id
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区

    int iCbRet = -1;                        //定义一个返回值
    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    if (cJSON_HasObjectItem(root, "adminId")) {
        //将json中adminId打印（复制）到stAdmin.sAdminId
        int flag = snprintf(sAdminId, sizeof(sAdminId), "%s",
                            cJSON_GetObjectItem(root, "adminId")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -5;
            strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
        }
    } else {
        stAdminResponse.iErrorCode = -5;
        strcpy(stAdminResponse.sErrorDetail, "NO_adminId");
    }

    printf("sAdminId:%s\n", sAdminId);

    //将查询adminId信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_admin where adminId=\"%s\";",
                 sAdminId) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /* 查询数据库是否存在adminId对应的管理员 */
    db_query(*ppvDBHandle, szSqlBuffer, admin_modifyPassword_in_callback, &iCbRet);
    //iCbRet为0即adminId对应的管理员是存在  否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {

        //将删除管理员命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "delete from lib_admin where adminId = \"%s\";", sAdminId) <=
            0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        /* 删除管理员 */
        iCbRet = db_update(*ppvDBHandle, szSqlBuffer);
        //如果iCbRet为0，则删除成功 否则返回admin_response_t错误信息
        if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = -12;
            strcpy(stAdminResponse.sErrorDetail, "DELETE_FAILURED");
        }
        //如果iCbRet为0，则删除成功
        if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0k");
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 1004);
    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
}


//创建数据库指针
void **get_handle() {
    static void *pvDBHandle = NULL;
    return &pvDBHandle;
}

//传入一个json字符串pRequest，并告诉长度iReqLen， 并传回一个回应字符串pResponse及其长度iResLen
//此函数主要功能为解析 解析json字符串，并根据解析后的结果选择实现相应的业务
int exec_business(const char *pRequest, int iReqLen, char *pResponse, int iResLen) {
    int iRet;
    int nMessageId;

    // 解析json字符串
    cJSON *root = cJSON_Parse(pRequest);
    if (NULL == root) return -1;

    if (!cJSON_HasObjectItem(root, "messageId")) {
        printf("have no messageId\n");
        return -1;
    }
    nMessageId = cJSON_GetObjectItem(root, "messageId")->valueint;
    switch (nMessageId) {
        case 1001:
            admin_login_in(root, pResponse, iResLen);
            break;
        case 1002:
            admin_register_in(root, pResponse, iResLen);
            break;
        case 1003:
            admin_modifyPassword_in(root, pResponse, iResLen);
            break;
        case 1004:
            admin_logout_in(root, pResponse, iResLen);
            break;
        default:
            printf("have no this message:%d\n", nMessageId);
            break;
    }

    cJSON_Delete(root);
    return 0;
}


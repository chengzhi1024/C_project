#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "business.h"
#include "db_mysql.h"
#include "cJSON.h"

//返回函数
//1.1管理员登录返回函数
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

//1.2管理员注册返回函数
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

//1.3管理员密码修改返回函数
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

//1.4管理员注销返回函数
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

//3.1用户借书返回函数
//3.1.1用户借书返回剩余函数
static int user_borrow_book_in_callback_remain(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    printf("row[%d] col[%d] value [%s]\n", iRow, 0, ppcArgs[0]);

    pRet = ppcArgs[0];

    return pRet;
}
//3.1.2用户借书返回剩余函数
static int user_borrow_book_in_callback_book_item(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    printf("row[%d] col[%d] value [%s]\n", iRow, 0, ppcArgs[0]);

    pRet = ppcArgs[0];

    return pRet;
}


//5.1添加用户返回函数
static int user_add_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
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

//5.2用户鉴权返回函数
static int user_authentication_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 1; i < iCol; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}

//5.3删除用户返回函数
static int delete_user_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
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


//业务函数
//1.1管理员登录函数
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

//1.2管理员注册函数
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
        iCbRet = -1;
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

//1.3管理员密码修改函数
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
        iCbRet = -1;
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
            strcpy(stAdminResponse.sErrorDetail, "UPDATE_FAILURED");
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

//1.4管理员注销函数
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
    db_query(*ppvDBHandle, szSqlBuffer, admin_logout_in_callback, &iCbRet);
    //iCbRet为0即adminId对应的管理员是存在  否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        iCbRet = -1;
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


////2.2图书入库函数
//static void Storage_book_in(cJSON *root, char *pResponse, int iResLen) {
//    /*
//     * "messageId": 2002
//     * "bookId": "ISBN"
//     * "bookName" : "书名",
//        "bookAuthor" : "作者",
//        "bookAddNumber" : 50,
//        "bookCategory" : "分类",
//        "bookPublisher" : "出版社",
//        "bookPublicationDate" : "2019-06-25 10:27:11"
//     */
//    book_item_info_t bookItemInfo;        //创建一个图书信息结构体
//    void **ppvDBHandle = get_handle();      //创建一个数据库句柄
//    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
//    int ibookAddNumber;
//
//    int iCbRet = -1;                        //定义一个返回值
//    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
//    stAdminResponse.iErrorCode = -100;
//    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
//
//
//    //判断 json对象中是否含有某一个元素  有：1， 无：0
//    //如果有就赋值给管理员结构体
//    if (cJSON_HasObjectItem(root, "bookId")) {
//        int flag = snprintf(bookItemInfo.sbookId, sizeof(bookItemInfo.sbookId), "%s",
//                            cJSON_GetObjectItem(root, "adminId")->valuestring);
//        if (flag < 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        } else if (flag == 0) {
//            stAdminResponse.iErrorCode = -21;
//            strcpy(stAdminResponse.sErrorDetail, "NO_bookId");
//        }
//    } else {
//        stAdminResponse.iErrorCode = -21;
//        strcpy(stAdminResponse.sErrorDetail, "NO_bookId");
//    }
//    if (cJSON_HasObjectItem(root, "bookName")) {
//        int flag = snprintf(bookItemInfo.sbookName, sizeof(bookItemInfo.sbookName), "%s",
//                            cJSON_GetObjectItem(root, "bookName")->valuestring);
//        if (flag < 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        } else if (flag == 0) {
//            stAdminResponse.iErrorCode = -22;
//            strcpy(stAdminResponse.sErrorDetail, "NO_bookName");
//        }
//    } else {
//        stAdminResponse.iErrorCode = -22;
//        strcpy(stAdminResponse.sErrorDetail, "NO_bookName");
//    }
//    if (cJSON_HasObjectItem(root, "bookAuthor")) {
//        int flag = snprintf(bookItemInfo.sbookAuthor, sizeof(bookItemInfo.sbookAuthor), "%s",
//                            cJSON_GetObjectItem(root, "bookAuthor")->valuestring);
//        if (flag < 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        } else if (flag == 0) {
//            stAdminResponse.iErrorCode = -23;
//            strcpy(stAdminResponse.sErrorDetail, "NO_bookAuthor");
//        }
//    } else {
//        stAdminResponse.iErrorCode = -23;
//        strcpy(stAdminResponse.sErrorDetail, "NO_bookAuthor");
//    }
//    if (cJSON_HasObjectItem(root, "bookAddNumber")) {
//        ibookAddNumber = cJSON_GetObjectItem(root, "bookAddNumber")->valueint;
//    }
//    if (cJSON_HasObjectItem(root, "bookCategory")) {
//        int flag = snprintf(bookItemInfo.sbookCategory, sizeof(bookItemInfo.sbookCategory), "%s",
//                            cJSON_GetObjectItem(root, "bookCategory")->valuestring);
//        if (flag < 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        } else if (flag == 0) {
//            stAdminResponse.iErrorCode = -24;
//            strcpy(stAdminResponse.sErrorDetail, "NO_bookCategory");
//        }
//    } else {
//        stAdminResponse.iErrorCode = -24;
//        strcpy(stAdminResponse.sErrorDetail, "NO_bookCategory");
//    }
//    if (cJSON_HasObjectItem(root, "bookPublisher")) {
//        int flag = snprintf(bookItemInfo.sbookPublisher, sizeof(bookItemInfo.sbookPublisher), "%s",
//                            cJSON_GetObjectItem(root, "bookPublisher")->valuestring);
//        if (flag < 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        } else if (flag == 0) {
//            stAdminResponse.iErrorCode = -25;
//            strcpy(stAdminResponse.sErrorDetail, "NO_bookPublisher");
//        }
//    } else {
//        stAdminResponse.iErrorCode = -25;
//        strcpy(stAdminResponse.sErrorDetail, "NO_bookPublisher");
//    }
//    if (cJSON_HasObjectItem(root, "bookPublicationDate")) {
//        int flag = snprintf(bookItemInfo.sbookPublicationDate, sizeof(bookItemInfo.sbookPublicationDate), "%s",
//                            cJSON_GetObjectItem(root, "bookPublicationDate")->valuestring);
//        if (flag < 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        } else if (flag == 0) {
//            stAdminResponse.iErrorCode = -26;
//            strcpy(stAdminResponse.sErrorDetail, "NO_bookPublicationDate");
//        }
//    } else {
//        stAdminResponse.iErrorCode = -26;
//        strcpy(stAdminResponse.sErrorDetail, "NO_bookPublicationDate");
//    }
//    //打印图书信息结构体数据
//    printf("bookId:%s,bookName:%s,bookAuthor:%s，bookAddNumber:%d,bookCategory:%s,bookPublisher:%s,bookPublicationDate:%s\n",
//           bookItemInfo.sbookId, bookItemInfo.sbookName, bookItemInfo.sbookAuthor, ibookAddNumber,
//           bookItemInfo.sbookCategory,
//           bookItemInfo.sbookPublisher, bookItemInfo.sbookPublicationDate);
//
//    //将sql插入命令打印（复制）到字符串szSqlBuffer
//    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "insert into lib_book values ('%s','%s','%s','%d','%s','%s','%s');",
//             bookItemInfo.sbookId, bookItemInfo.sbookName, bookItemInfo.sbookAuthor, ibookAddNumber,
//             bookItemInfo.sbookCategory,
//             bookItemInfo.sbookPublisher, bookItemInfo.sbookPublicationDate);
//
//    //插入表业务
//    /* 插入图书信息 */
//    iCbRet = db_insert(*ppvDBHandle, szSqlBuffer);
//    //如果iCbRet为0，则插入成功 否则返回admin_response_t错误信息
//    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
//        stAdminResponse.iErrorCode = -8;
//        strcpy(stAdminResponse.sErrorDetail, "INSERT_FAILURED");
//    }
//    //如果iCbRet为0，则插入成功
//    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
//        stAdminResponse.iErrorCode = 0;
//        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0k");
//    }
//
//    /* 组装数据库为json字符串 */
//    cJSON *json = cJSON_CreateObject();
//    if (!json) return;
//    cJSON_AddNumberToObject(json, "messageId", 2002);
//    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
//    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);
//
//    char *pResJson = cJSON_PrintUnformatted(json);
//    snprintf(pResponse, iResLen, "%s", pResJson);
//
//    cJSON_free(pResJson);
//    cJSON_Delete(json);
//}
//


//3.1用户借书函数
static void user_borrow_book_in(cJSON *root, char *pResponse, int iResLen) {

    borrowORreturn_book_info_t stBorrowBook; //创建一个借书信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区

    int iCbRet = -1;                        //定义一个返回值
    borrowORreturn_book_response_t stBorrowBookResponse;       //创建一个返回信息结构体 并赋初值
    stBorrowBookResponse.iErrorCode = -100;
    strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给借书信息结构体
    if (cJSON_HasObjectItem(root, "userNum")) {
        //将json中userNum打印（复制）到stBorrowBook.sUserNum
        int flag = snprintf(stBorrowBook.sUserNum, sizeof(stBorrowBook.sUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stBorrowBookResponse.iErrorCode = -101;
            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stBorrowBookResponse.iErrorCode = -13;
            strcpy(stBorrowBookResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stBorrowBookResponse.iErrorCode = -13;
        strcpy(stBorrowBookResponse.sErrorDetail, "NO_userNum");
    }
    if (cJSON_HasObjectItem(root, "sbookId")) {
        int flag = snprintf(stBorrowBook.sbookId, sizeof(stBorrowBook.sbookId), "%s",
                            cJSON_GetObjectItem(root, "sbookId")->valuestring);
        if (flag < 0) {
            stBorrowBookResponse.iErrorCode = -101;
            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stBorrowBookResponse.iErrorCode = -21;
            strcpy(stBorrowBookResponse.sErrorDetail, "NO_bookId");
        }
    } else {
        stBorrowBookResponse.iErrorCode = -21;
        strcpy(stBorrowBookResponse.sErrorDetail, "NO_bookId");
    }

    printf("sUserNum:%s,sbookId:%s\n", stBorrowBook.sUserNum, stBorrowBook.sbookId);

    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select lib_book.bookRemain from lib_book where bookId = \"%s\";",
                 stBorrowBook.sbookId) <= 0) {
        stBorrowBookResponse.iErrorCode = -101;
        strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询数据库是否存在bookId对应的图书
    db_query(*ppvDBHandle, szSqlBuffer, user_borrow_book_in_callback_remain, &iCbRet);
    //iCbRet为正数或0，即bookId对应的图书存在 否则返回borrowORreturn_book_response_t错误信息
    if (0 > iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
        stBorrowBookResponse.iErrorCode = -22;
        strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_NO_BOOK");
    } else {
        //图书存在 先判断用户是否存在
        //将查询信息打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum =\"%s\";",
                     stBorrowBook.sUserNum) <= 0) {
            stBorrowBookResponse.iErrorCode = -101;
            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
        }
        //查询数据库是否存在usertNum对应的用户
        int flag = db_query(*ppvDBHandle, szSqlBuffer, user_authentication_in_callback, &iCbRet);
        //iCbRet为0，即UserNum对应的用户存在 否则返回borrowORreturn_book_response_t错误信息
        if (0 != flag && -100 == stBorrowBookResponse.iErrorCode) {
            stBorrowBookResponse.iErrorCode = -3;
            strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_NO_USER");
        }
        if (0 == flag && -100 == stBorrowBookResponse.iErrorCode) {
            //用户存在 判断剩余
            if (0 < iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
                //剩余充足 可以借书
                iCbRet = -1;
                //将修改信息打印（复制）到字符串szSqlBuffer
                if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                             "update lib_book set bookRemain = bookRemain - 1 where bookId = \"%s\";",
                             stBorrowBook.sUserNum) <= 0) {
                    stBorrowBookResponse.iErrorCode = -101;
                    strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                }
                //修改剩余信息
                iCbRet = db_update(*ppvDBHandle, szSqlBuffer);
                if (0 != iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
                    stBorrowBookResponse.iErrorCode = -11;
                    strcpy(stBorrowBookResponse.sErrorDetail, "UPDATE_FAILURED");
                }
                if (0 == iCbRet && -100 == stBorrowBookResponse.iErrorCode) {

                    //插入一条借书信息
                    insert into lib_book_borrow (borId,userId,bookId,borStatus,borDate,borRetDateLimit,retDate) values ('9','3','2004','1','2015-09-10 00:00:00','2015-12-10 00:00:00','2015-10-25 00:00:00');
                    iCbRet = -1;
                    //将修改信息打印（复制）到字符串szSqlBuffer
                    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                 "insert into lib_book_borrow (borId,userId,bookId,borStatus,borDate,borRetDateLimit) values (\"%s\",\"%s\",\"%s\",'1',\"%s\",\"%s\");",
                                 stBorrowBook.sUserNum,stBorrowBook.sUserNum,stBorrowBook.sbookId,   gettimeofday(),stBorrowBook.sUserNum) <= 0) {
                        stBorrowBookResponse.iErrorCode = -101;
                        strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                    }
                    //插入借阅信息
                    iCbRet = db_insert(*ppvDBHandle, szSqlBuffer);
                    if(0 != iCbRet && -100 == stBorrowBookResponse.iErrorCode){
                        stBorrowBookResponse.iErrorCode = -8;
                        strcpy(stBorrowBookResponse.sErrorDetail, "INSERT_FAILURED");
                    }
                   if(0 == iCbRet && -100 == stBorrowBookResponse.iErrorCode){
                       stBorrowBookResponse.iErrorCode = 0;
                       strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_0k");
                   }
                }
            }
            //剩余不足 不能借书
            if (0 == iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
                stBorrowBookResponse.iErrorCode = -23;
                strcpy(stBorrowBookResponse.sErrorDetail, "BOOK_BORROW_OUT");
            }
        }
        //借书完成 打印当前用户的借阅信息
        //将查询信息打印（复制）到字符串szSqlBuffer
        flag=-1;
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                     "select bookName,userNum,bookAuthor,bookCategory,bookPublisher,bookPublicationDate,borRetDateLimit,borDate from (lib_book_borrow left join lib_book on lib_book_borrow.bookId = lib_book.bookId) left join lib_user on lib_book_borrow.userId = lib_user.userId where  lib_user.userNum =\"%s\";",
                     stBorrowBook.sUserNum) <= 0) {
            stBorrowBookResponse.iErrorCode = -101;
            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
        }
        //查询数据库是否存在usertNum对应的用户借阅信息
        flag = db_query(*ppvDBHandle, szSqlBuffer, user_borrow_book_in_callback_book_item, &iCbRet);
        if (0 != flag && -100 == stBorrowBookResponse.iErrorCode) {
            stBorrowBookResponse.iErrorCode = -24;
            strcpy(stBorrowBookResponse.sErrorDetail, "QUERY_FAILURED");
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 3001);
    cJSON_AddNumberToObject(json, "total", 20);
    cJSON_AddNumberToObject(json, "item", 2222);


//    borrowORreturn_book_info_t

    cJSON_AddNumberToObject(json, "errorCode", stBorrowBookResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stBorrowBookResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
}


//5.1添加用户函数
static void user_add_in(cJSON *root, char *pResponse, int iResLen) {
    user_info_t stUser = {""};         //创建一个用户信息结构体
    void **ppvDBHandle = get_handle();          //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];     //创建一个数据库命令缓冲区

    int iCbRet = -1;                            //定义一个返回值
    admin_response_t stAdminResponse;           //定义一个返回信息结构体，并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断json对象是否含有元素
    //如果有就赋值给用户信息结构体
    if (cJSON_HasObjectItem(root, "userNum")) {
        //将json中userNum赋值给用户信息结构体
        int flag = snprintf(stUser.sUserNum, sizeof(stUser.sUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -13;
            strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stAdminResponse.iErrorCode = -13;
        strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
    }
    if (cJSON_HasObjectItem(root, "userName")) {
        int flag = snprintf(stUser.sUserName, sizeof(stUser.sUserName), "%s",
                            cJSON_GetObjectItem(root, "userName")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -14;
            strcpy(stAdminResponse.sErrorDetail, "NO_userName");
        }
    } else {
        stAdminResponse.iErrorCode = -14;
        strcpy(stAdminResponse.sErrorDetail, "NO_userName");
    }
    if (cJSON_HasObjectItem(root, "userSex")) {
        int flag = snprintf(stUser.sUserSex, sizeof(stUser.sUserSex), "%s",
                            cJSON_GetObjectItem(root, "userSex")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
//        else if (flag == 0) {
//            stAdminResponse.iErrorCode = -15;
//            strcpy(stAdminResponse.sErrorDetail, "NO_userSex");
//        }
    }
//    else {
//        stAdminResponse.iErrorCode = -15;
//        strcpy(stAdminResponse.sErrorDetail, "NO_userSex");
//    }
    if (cJSON_HasObjectItem(root, "userRemark")) {
        int flag = snprintf(stUser.sUserRemark, sizeof(stUser.sUserRemark), "%s",
                            cJSON_GetObjectItem(root, "userRemark")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
//        else if (flag == 0) {
//            stAdminResponse.iErrorCode = -16;
//            strcpy(stAdminResponse.sErrorDetail, "NO_userRemark");
//        }
    }
//    else {
//        stAdminResponse.iErrorCode = -16;
//        strcpy(stAdminResponse.sErrorDetail, "NO_userRemark");
//    }

    printf("sUserNum:%s,sUserName:%s,sUserSex:%s, sUserRemark:%s\n",
           stUser.sUserNum, stUser.sUserName, stUser.sUserSex, stUser.sUserRemark);

    //组装sql命令
    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum = '%s';",
                 stUser.sUserNum) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询数据库是否存在相同的userNum对应的用户
    db_query(*ppvDBHandle, szSqlBuffer, user_add_in_callback, &iCbRet);
    //iCbRet为0即userNum对应的用户是存在 返回admin_response_t错误信息 否则添加一个用户
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -17;
        strcpy(stAdminResponse.sErrorDetail, "USER_ALREADY_EXIST");
    }
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        iCbRet = -1;
        //将插入命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                     "insert into lib_user (userNum, userName, userSex,userRemark) values ('%s','%s','%s','%s');",
                     stUser.sUserNum, stUser.sUserName, stUser.sUserSex, stUser.sUserRemark) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        //插入用户
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
    cJSON_AddNumberToObject(json, "messageId", 5001);
    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
}

//5.2用户鉴权函数
static void user_authentication_in(cJSON *root, char *pResponse, int iResLen) {

    char stUserNum[24];;                     //创建一个用户id数组
    void **ppvDBHandle = get_handle();       //创建一个数据库句柄
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];  //创建一个数据库命令缓冲区

    int iCbRet = -1;                         //定义一个返回值
    admin_response_t stAdminResponse;        //定义一个返回信息结构体
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断json对象是否有userNum
    if (cJSON_HasObjectItem(root, "userNum")) {
        //userNum（复制）到stUserNum
        int flag = snprintf(stUserNum, sizeof(stUserNum), "%s", cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -13;
            strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stAdminResponse.iErrorCode = -13;
        strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
    }

    printf("sUserNum:%s\n", stUserNum);

    //组装sql命令
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum ='%s';",
                 stUserNum) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询是否有相同的用户
    db_query(*ppvDBHandle, szSqlBuffer, user_authentication_in_callback, &iCbRet);
    //iCbRet为0，即adminId对应的管理员存在 否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = 0;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0k");
    }

    // 组装数据库为json字符串
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 5002);
    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);

}

//5.3删除用户函数
static void delete_user_in(cJSON *root, char *pResponse, int iResLen) {
    char stUserNum[24];                                  //创建一个用户id数组
    void **ppvDBHandle = get_handle();                  //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];             //创建一个数据库命令缓冲区

    int iCbRet = -1;                                    //定义一个返回值
    admin_response_t stAdminResponse;                   //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    if (cJSON_HasObjectItem(root, "userNum")) {
        //将json中userNum赋值给stUserNum
        int flag = snprintf(stUserNum, sizeof(stUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -13;
            strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stAdminResponse.iErrorCode = -13;
        strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
    }

    printf("sUserNum:%s\n", stUserNum);

    //将查询UserNum信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum =\"%s\";",
                 stUserNum) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /* 查询数据库是否存在UserNum对应的用户 */
    db_query(*ppvDBHandle, szSqlBuffer, delete_user_in_callback, &iCbRet);
    //iCbRet为0即adminId对应的用户存在  否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        iCbRet = -1;
        //将删除用户命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "delete from lib_user where userNum = \"%s\";", stUserNum) <=
            0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        /* 删除用户 */
        iCbRet = db_update(*ppvDBHandle, szSqlBuffer);
        //如果iCbRet为0，则删除成功 否则返回admin_response_t错误信息
        if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = -12;
            strcpy(stAdminResponse.sErrorDetail, "DELETE_FAILURED");
        }
        //如果iCbRet为0，则删除成功
        if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0K");
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 5003);
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
        case 2002:
            Storage_book_in(root, pResponse, iResLen);
            break;
        case 5001:
            user_add_in(root, pResponse, iResLen);
            break;
        case 5002:
            user_authentication_in(root, pResponse, iResLen);
            break;
        case 5003:
            delete_user_in(root, pResponse, iResLen);
            break;
        default:
            printf("have no this message:%d\n", nMessageId);
            /* 组装数据库为json字符串 */
            cJSON *json = cJSON_CreateObject();
            if (!json) return 0;
            cJSON_AddNumberToObject(json, "messageId", 0000);
            cJSON_AddNumberToObject(json, "errorCode", -1111);
            cJSON_AddStringToObject(json, "errorDetail", "NO_MESSAGE");

            char *pResJson = cJSON_PrintUnformatted(json);
            snprintf(pResponse, iResLen, "%s", pResJson);

            cJSON_free(pResJson);
            break;
    }


    cJSON_Delete(root);
    return 0;
}


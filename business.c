//
// Created by yang on 2019/6/24.
//

#include "business.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "db_mysql/db_interface.h"
#include <cJSON.h>




static int Search_book_in_callback(char **ppcArgs, int iRow, int iCol, void* pvData)
{
    book_item_info_t *pItems = (book_item_info_t *)pvData;

    if(1 != iCol) return -1;
    snprintf(pItems[iRow-1].sbookId, sizeof(pItems[iRow-1].sbookId),"%s",ppcArgs[0]);
    snprintf(pItems[iRow-1].sbookName, sizeof(pItems[iRow-1].sbookName),"%s",ppcArgs[1]);
    snprintf(pItems[iRow-1].sbookAuthor, sizeof(pItems[iRow-1].sbookAuthor),"%s",ppcArgs[2]);
    pItems[iRow-1].ibookStock = atoi(ppcArgs[3]);
    pItems[iRow-1].ibookRemain = atoi(ppcArgs[4]);
    pItems[iRow-1].ibookTimes = atoi(ppcArgs[5]);
    snprintf(pItems[iRow-1].sbookCategory, sizeof(pItems[iRow-1].sbookCategory),"%s",ppcArgs[6]);
    snprintf(pItems[iRow-1].sbookPublisher, sizeof(pItems[iRow-1].sbookPublisher),"%s",ppcArgs[7]);
    snprintf(pItems[iRow-1].sbookPublicationDate, sizeof(pItems[iRow-1].sbookPublicationDate),"%s",ppcArgs[8]);

    return 0;
}
/** 登录请求的回调函数
 * 打印每行的数据
 * @param ppcArgs
 * @param iRow
 * @param iCol
 * @param pvData
 * @return
 */

static int admin_login_in_callback(char **ppcArgs, int iRow, int iCol, void* pvData)
{
    int i;
    int *pRet = (int *)pvData;

    *pRet = 0;
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 0; i < iCol; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}

/**
 * @brief 管理员登录函数
 * @param root  管理员json对象
 * @param pResponse  回应的消息
 * @param iResLen
 */
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
        strcpy(stAdmin.sAdminId, cJSON_GetObjectItem(root, "adminId")->valuestring);
    }
    if (cJSON_HasObjectItem(root, "adminPasswd")) {
        strcpy(stAdmin.sAdminPasswd, cJSON_GetObjectItem(root, "adminPasswd")->valuestring);
    }
    if (cJSON_HasObjectItem(root, "adminName")) {
        strcpy(stAdmin.sAdminName, cJSON_GetObjectItem(root, "adminName")->valuestring);
    }


    //将sql命令信息打印（复制）到字符串szSqlBuffer
    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_admin where adminId=\"%s\";",
             stAdmin.sAdminId);

    /* 查询数据库是否存在adminId对应的管理员 */
    db_query( *ppvDBHandle, szSqlBuffer, admin_login_in_callback, &iCbRet);
    //如果admin_login_in_callback返回值iCbRet不为0，即adminId对应的管理员是否存在 否则返回admin_response_t错误信息
    if (0 != iCbRet) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }

    if (0 == iCbRet) {
        iCbRet = -1;
        //继续生成查询用户的其他信息的命令
        snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                 "select * from lib_admin where adminId=\"%s\" and adminPasswd=\"%s\";",
                 stAdmin.sAdminId, stAdmin.sAdminPasswd);
        //继续查询用户的其他信息是否存在 用户名和密码是否正确
        db_query(*ppvDBHandle, szSqlBuffer, admin_login_in_callback, (void *) &iCbRet);
        //再次判断用户名和密码是否正确，否则返回admin_response_t
        if (0 != iCbRet) {
            stAdminResponse.iErrorCode = -1;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_PASSWD_ERROR");
        } else {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "success");
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return ;
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
    printf("row[%d] col[%d] value [%s]\n", iRow, 0, ppcArgs[0]);
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
        strcpy(stAdmin.sAdminId, cJSON_GetObjectItem(root, "adminId")->valuestring);
    }
    if (cJSON_HasObjectItem(root, "adminPasswd")) {
        strcpy(stAdmin.sAdminPasswd, cJSON_GetObjectItem(root, "adminPasswd")->valuestring);
    }
    if (cJSON_HasObjectItem(root, "adminName")) {
        strcpy(stAdmin.sAdminName, cJSON_GetObjectItem(root, "adminName")->valuestring);
    }

    printf("sAdminId:%s,sAdminPasswd:%s,sAdminName:%s\n",
           stAdmin.sAdminId, stAdmin.sAdminPasswd, stAdmin.sAdminName);

    //将查询信息打印（复制）到字符串szSqlBuffer
    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "insert into lib_admin values ('%s','%s','%s');",
             stAdmin.sAdminId, stAdmin.sAdminPasswd, stAdmin.sAdminName);


    /* 查询数据库是否存在adminId对应的管理员 */
    db_query(*ppvDBHandle, szSqlBuffer, admin_login_in_callback, &iCbRet);
    if (0 == iCbRet) {
        stAdminResponse.iErrorCode = -4;
        strcpy(stAdminResponse.sErrorDetail, "ADMIN_ALREADY_EXIST");
    }
    if (0 != iCbRet) {
        /* 插入管理员 */
        iCbRet = db_insert(*ppvDBHandle, szSqlBuffer);
        //如果iCbRet不为0，则插入成功 否则返回admin_response_t错误信息
        if (0 != iCbRet) {
            stAdminResponse.iErrorCode = -3;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
        }
        //如果iCbRet为0，则插入成功
        if (0 == iCbRet) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0K");
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

//管理员注销函数
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

//显示所有书籍的数目回调函数
static int total_book_callback(char **ppcArgs,int iRow, int iCol, void* pvData)
{
    int *iRet = (int *)pvData;
    *iRet = atoi(ppcArgs[0]);
}

//显示图书回调函数
static int display_book_callback(char **ppcArgs, int iRow, int iCol, void* pvData)
{

    book_item_info_t *pItems = (book_item_info_t *)pvData;

    if(9 != iCol) return -1;
    snprintf(pItems[iRow-1].sbookId, sizeof(pItems[iRow-1].sbookId),"%s",ppcArgs[0]);
    snprintf(pItems[iRow-1].sbookName, sizeof(pItems[iRow-1].sbookName),"%s",ppcArgs[1]);
    snprintf(pItems[iRow-1].sbookAuthor, sizeof(pItems[iRow-1].sbookAuthor),"%s",ppcArgs[2]);
    pItems[iRow-1].ibookStock = atoi(ppcArgs[3]);
    if(NULL == ppcArgs[4])
    {
        pItems[iRow-1].ibookRemain = 0;
    }else {pItems[iRow-1].ibookRemain = atoi(ppcArgs[4]);}
    if(NULL == ppcArgs[4])
    {
        pItems[iRow-1].ibookTimes = 0;
    }else {pItems[iRow-1].ibookTimes = atoi(ppcArgs[5]);}
    snprintf(pItems[iRow-1].sbookCategory, sizeof(pItems[iRow-1].sbookCategory),"%s",ppcArgs[6]);
    snprintf(pItems[iRow-1].sbookPublisher, sizeof(pItems[iRow-1].sbookPublisher),"%s",ppcArgs[7]);
    snprintf(pItems[iRow-1].sbookPublicationDate, sizeof(pItems[iRow-1].sbookPublicationDate),"%s",ppcArgs[8]);

    return 0;
}

//显示图书函数
static void display_book_in(cJSON *root, char *pResponse, int iResLen) {
// "messageId" : 2001
// "start" : 2
// "end" : 10
    int iTotal = 0;
    int iStart, iEnd;
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];
    void **ppvDBHandle = get_handle();

//解析json
    if (cJSON_HasObjectItem(root, "start")) {
        iStart = cJSON_GetObjectItem(root, "start")->valueint;
    }
    if (cJSON_HasObjectItem(root, "end")) {
        iEnd = cJSON_GetObjectItem(root, "end")->valueint;
    }
//组装sql命令
    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book;");
//查询数据库，查找所有记录数
    db_query(*ppvDBHandle, szSqlBuffer, total_book_callback, (void *)&iTotal);
    printf("%d\n",iTotal);
//组装sql查询命令
    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book limit %d,%d;", iStart, iEnd);
//查询数据库
    book_item_info_t *pItems = (book_item_info_t *)malloc(sizeof(book_item_info_t)*iEnd);
    db_query(*ppvDBHandle, szSqlBuffer, display_book_callback, (void *)pItems);

//转化为json字符串
    cJSON *json = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();

    for (int (i) = 0; (i) < 9; i++) {
        cJSON *pRowJson = cJSON_CreateObject();
        cJSON_AddStringToObject(pRowJson, "sbookId", pItems[i].sbookId);
        cJSON_AddStringToObject(pRowJson, "sbookName", pItems[i].sbookName);
        cJSON_AddStringToObject(pRowJson, "sbookAuthor", pItems[i].sbookAuthor);
        cJSON_AddNumberToObject(pRowJson, "ibookStock", pItems[i].ibookStock);
        cJSON_AddNumberToObject(pRowJson, "ibookRemain", pItems[i].ibookRemain);
        cJSON_AddNumberToObject(pRowJson, "ibookTimes", pItems[i].ibookTimes);
        cJSON_AddStringToObject(pRowJson, "sbookCategory", pItems[i].sbookCategory);
        cJSON_AddStringToObject(pRowJson, "sbookPublisher", pItems[i].sbookPublisher);
        cJSON_AddStringToObject(pRowJson, "sbookPublicationDate", pItems[i].sbookPublicationDate);
        cJSON_AddItemToArray(array,pRowJson);
    }

    cJSON_AddNumberToObject(json, "messageId", 2001);
    cJSON_AddNumberToObject(json, "total", iTotal);
    cJSON_AddItemToObject(json,"item",array);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    free(pItems);
    cJSON_free(pResJson);
    cJSON_Delete(json);
}

//图书入库回调函数
static int storage_book_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的bookId的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 0; i < 1; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}

//图书入库函数
static void storage_book_in(cJSON *root, char *pResponse, int iResLen)
{
    /*
     * "messageId": 2002
     * "bookId": "ISBN"
     * "bookName" : "书名",
        "bookAuthor" : "作者",
        "bookAddNumber" : 50,
        "bookCategory" : "分类",
        "bookPublisher" : "出版社",
        "bookPublicationDate" : "2019-06-25 10:27:11"
     */
    book_item_info_t bookItemInfo;        //创建一个图书信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库句柄
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
    int ibookAddNumber;

    int iCbRet = -1;                        //定义一个返回值
    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");


    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给管理员结构体
    if (cJSON_HasObjectItem(root, "bookId")) {

        int flag = snprintf(bookItemInfo.sbookId, sizeof(bookItemInfo.sbookId), "%s",
                            cJSON_GetObjectItem(root, "bookId")->valuestring);

        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -21;
            strcpy(stAdminResponse.sErrorDetail, "NO_bookId");
        }
    } else {
        stAdminResponse.iErrorCode = -21;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookId");
    }
    if (cJSON_HasObjectItem(root, "bookName")) {
        int flag = snprintf(bookItemInfo.sbookName, sizeof(bookItemInfo.sbookName), "%s",
                            cJSON_GetObjectItem(root, "bookName")->valuestring);
        printf("%s\n",bookItemInfo.sbookName);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -22;
            strcpy(stAdminResponse.sErrorDetail, "NO_bookName");
        }
    } else {
        stAdminResponse.iErrorCode = -22;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookName");
    }
    if (cJSON_HasObjectItem(root, "bookAuthor")) {
        int flag = snprintf(bookItemInfo.sbookAuthor, sizeof(bookItemInfo.sbookAuthor), "%s",
                            cJSON_GetObjectItem(root, "bookAuthor")->valuestring);
        printf("%s\n",bookItemInfo.sbookAuthor);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -23;
            strcpy(stAdminResponse.sErrorDetail, "NO_bookAuthor");
        }
    } else {
        stAdminResponse.iErrorCode = -23;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookAuthor");
    }
    if (cJSON_HasObjectItem(root, "bookAddNumber")) {
        ibookAddNumber = cJSON_GetObjectItem(root, "bookAddNumber")->valueint;
    }
    if (cJSON_HasObjectItem(root, "bookCategory")) {
        int flag = snprintf(bookItemInfo.sbookCategory, sizeof(bookItemInfo.sbookCategory), "%s",
                            cJSON_GetObjectItem(root, "bookCategory")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -24;
            strcpy(stAdminResponse.sErrorDetail, "NO_bookCategory");
        }
    } else {
        stAdminResponse.iErrorCode = -24;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookCategory");
    }
    if (cJSON_HasObjectItem(root, "bookPublisher")) {
        int flag = snprintf(bookItemInfo.sbookPublisher, sizeof(bookItemInfo.sbookPublisher), "%s",
                            cJSON_GetObjectItem(root, "bookPublisher")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -25;
            strcpy(stAdminResponse.sErrorDetail, "NO_bookPublisher");
        }
    } else {
        stAdminResponse.iErrorCode = -25;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookPublisher");
    }
    if (cJSON_HasObjectItem(root, "bookPublicationDate")) {
        int flag = snprintf(bookItemInfo.sbookPublicationDate, sizeof(bookItemInfo.sbookPublicationDate), "%s",
                            cJSON_GetObjectItem(root, "bookPublicationDate")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -26;
            strcpy(stAdminResponse.sErrorDetail, "NO_bookPublicationDate");
        }
    } else {
        stAdminResponse.iErrorCode = -26;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookPublicationDate");
    }
    //打印图书信息结构体数据
    printf("bookId:%s,bookName:%s,bookAuthor:%s，bookAddNumber:%d,bookCategory:%s,bookPublisher:%s,bookPublicationDate:%s\n",
           bookItemInfo.sbookId, bookItemInfo.sbookName, bookItemInfo.sbookAuthor,ibookAddNumber,bookItemInfo.sbookCategory,
           bookItemInfo.sbookPublisher,bookItemInfo.sbookPublicationDate);

    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookId= '%s';",
                 bookItemInfo.sbookId) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /* 查询数据库是否存在bookId对应的图书 */
    db_query(*ppvDBHandle, szSqlBuffer, storage_book_in_callback, &iCbRet);
    //iCbRet为0即bookId对应的图书是存在 返回admin_response_t错误信息 否则添加一个管理员
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        iCbRet = -1;
        //将更新命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "update lib_book set bookStock = bookStock + %d where bookId = '%s';",
                     ibookAddNumber, bookItemInfo.sbookId) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        /* 更新图书库存 */
        iCbRet = db_update(*ppvDBHandle, szSqlBuffer);
        //如果iCbRet为0，则更新成功 否则返回admin_response_t错误信息
        if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = -11;
            strcpy(stAdminResponse.sErrorDetail, "UPDATE_FAILURED");
        }
        //如果iCbRet为0，则更新成功
        if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0k");
        }

    }
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        iCbRet = -1;
        //将查询信息打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "insert into lib_book (bookId,bookName,bookAuthor,bookStock,bookCategory,bookPublisher,bookPublicationDate) values ('%s','%s','%s','%d','%s','%s','%s');",
                     bookItemInfo.sbookId, bookItemInfo.sbookName, bookItemInfo.sbookAuthor,ibookAddNumber,bookItemInfo.sbookCategory,
                     bookItemInfo.sbookPublisher,bookItemInfo.sbookPublicationDate) <= 0) {
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
    cJSON_AddNumberToObject(json, "messageId", 2002);
    cJSON_AddNumberToObject(json, "errorCode", stAdminResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stAdminResponse.sErrorDetail);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
}

//查询图书回调函数
static int search_book_in_callback(char **ppcArgs, int iRow, int iCol, void* pvData)
{

    book_item_info_t *pItems = (book_item_info_t *)pvData;


    if(9 != iCol) return -1;
    snprintf(pItems[iRow-1].sbookId, sizeof(pItems[iRow-1].sbookId),"%s",ppcArgs[0]);
    snprintf(pItems[iRow-1].sbookName, sizeof(pItems[iRow-1].sbookName),"%s",ppcArgs[1]);
    snprintf(pItems[iRow-1].sbookAuthor, sizeof(pItems[iRow-1].sbookAuthor),"%s",ppcArgs[2]);
    pItems[iRow-1].ibookStock = atoi(ppcArgs[3]);
    if(NULL == ppcArgs[4])
    {
        pItems[iRow-1].ibookRemain = 0;
    }else {pItems[iRow-1].ibookRemain = atoi(ppcArgs[4]);}
    if(NULL == ppcArgs[4])
    {
        pItems[iRow-1].ibookTimes = 0;
    }else {pItems[iRow-1].ibookTimes = atoi(ppcArgs[5]);}
    snprintf(pItems[iRow-1].sbookCategory, sizeof(pItems[iRow-1].sbookCategory),"%s",ppcArgs[6]);
    snprintf(pItems[iRow-1].sbookPublisher, sizeof(pItems[iRow-1].sbookPublisher),"%s",ppcArgs[7]);
    snprintf(pItems[iRow-1].sbookPublicationDate, sizeof(pItems[iRow-1].sbookPublicationDate),"%s",ppcArgs[8]);

    return 0;
}

//查询图书函数
static void search_book_in(cJSON *root, char *pResponse, int iResLen)
{
    //"messageId" : 2003,
    //"bookSearch" : "",
    //"type" : 0 // 0代表书号，1代表书名，2代表作者
    char cBookSearch[24];
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
    int iType,iTotal;

    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

//解析json对象
    if (cJSON_HasObjectItem(root, "bookSearch")) {
        int flag = snprintf(cBookSearch, sizeof(cBookSearch), "%s",
                            cJSON_GetObjectItem(root, "bookSearch")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -23;
            strcpy(stAdminResponse.sErrorDetail, "NO_bookAuthor");
        }
    } else {
        stAdminResponse.iErrorCode = -23;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookAuthor");
    }
    if (cJSON_HasObjectItem(root, "type")) {
        iType = cJSON_GetObjectItem(root, "type")->valueint;
    }

//打印数据测试是否解析成功
    printf("bookSearch:%s,iType: %d\n",cBookSearch,iType);

    //组装sql命令，先判断查询类型
    //一共有三种类型，0代表书号，1代表书名，2代表作者
    if(0 == iType){

        if (cJSON_HasObjectItem(root, "bookSearch")) {
            snprintf(cBookSearch, sizeof(cBookSearch), "%s",cJSON_GetObjectItem(root, "bookSearch")->valuestring);
        }
        //组装sql命令,确定条目总数
        snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book where bookName = '%s';",cBookSearch);
        //查询数据库，查找所有记录数
        db_query(*ppvDBHandle, szSqlBuffer, total_book_callback, (void *)&iTotal);
        snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookId = '%s';",cBookSearch);
    }
    else if(1 == iType){
        if (cJSON_HasObjectItem(root, "bookSearch")) {
            snprintf(cBookSearch, sizeof(cBookSearch), "%s",cJSON_GetObjectItem(root, "bookSearch")->valuestring);
        }
        //组装sql命令,确定条目总数
        snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book where bookName = '%s';",cBookSearch);
        //查询数据库，查找所有记录数
        db_query(*ppvDBHandle, szSqlBuffer, total_book_callback, (void *)&iTotal);
        snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookName = '%s';",cBookSearch);
    }
    else if(2 == iType){
        if (cJSON_HasObjectItem(root, "bookSearch")) {
            snprintf(cBookSearch, sizeof(cBookSearch), "%s",cJSON_GetObjectItem(root, "bookSearch")->valuestring);
        }
        //组装sql命令,确定条目总数
        snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book where bookAuthor = '%s';",cBookSearch);
        //查询数据库，查找所有记录数
        db_query(*ppvDBHandle, szSqlBuffer, total_book_callback, (void *)&iTotal);
        snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookAuthor = '%s';",cBookSearch);
    } else {
        stAdminResponse.iErrorCode = -100;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
    }
    //查询业务部分
    //查询数据库
    book_item_info_t *pItems = (book_item_info_t *) malloc(sizeof(book_item_info_t)*iTotal);
    db_query(*ppvDBHandle, szSqlBuffer, search_book_in_callback, (void *)pItems);
    printf("%d\n",iTotal);


    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();


    for (int (i) = 0; (i) < iTotal; i++) {
        cJSON *pRowJson = cJSON_CreateObject();
        cJSON_AddStringToObject(pRowJson, "sbookId", pItems[i].sbookId);
        cJSON_AddStringToObject(pRowJson, "sbookName", pItems[i].sbookName);
        cJSON_AddStringToObject(pRowJson, "sbookAuthor", pItems[i].sbookAuthor);
        cJSON_AddNumberToObject(pRowJson, "ibookStock", pItems[i].ibookStock);
        cJSON_AddNumberToObject(pRowJson, "ibookRemain", pItems[i].ibookRemain);
        cJSON_AddNumberToObject(pRowJson, "ibookTimes", pItems[i].ibookTimes);
        cJSON_AddStringToObject(pRowJson, "sbookCategory", pItems[i].sbookCategory);
        cJSON_AddStringToObject(pRowJson, "sbookPublisher", pItems[i].sbookPublisher);
        cJSON_AddStringToObject(pRowJson, "sbookPublicationDate", pItems[i].sbookPublicationDate);
        cJSON_AddItemToArray(array,pRowJson);
    }



    if (!json) return;
    cJSON_AddNumberToObject(json, "messageId", 2003);
    cJSON_AddNumberToObject(json, "total", iTotal);
    cJSON_AddItemToObject(json,"item",array);

    char *pResJson = cJSON_PrintUnformatted(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    free(pItems);
    cJSON_free(pResJson);
    cJSON_Delete(json);

}

void** get_handle()
{
    static void* pvDBHandle = NULL;
    return &pvDBHandle;
}

//添加用户业务函数
static void Add_user_in(cJSON *root, char *pResponse, int iResLen)
{
    /*
     *  "userNum" : "卡号",
        "userName" : "姓名",
        "userSex" : "性别",
        "userRemark" : "备注"
     */
    user_item_info_t userItemInfo;
    void **ppvDBHandle = get_handle();          //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];     //创建一个数据库命令缓冲区

    int iCbRet = -1;                            //定义一个返回值
    admin_response_t stAdminResponse;           //定义一个返回信息结构体，并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断json对象是否含有元素
    //如果有就赋值给管理员结构体
    if (cJSON_HasObjectItem(root, "userNum")) {
        //将json中userNum打印（复制）到
        int flag = snprintf(userItemInfo.sUserNum, sizeof(userItemInfo.sUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -5;
            strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stAdminResponse.iErrorCode = -5;
        strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
    }
    if (cJSON_HasObjectItem(root, "userName")) {
        int flag = snprintf(userItemInfo.sUserName, sizeof(userItemInfo.sUserName), "%s",
                            cJSON_GetObjectItem(root, "userName")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -6;
            strcpy(stAdminResponse.sErrorDetail, "NO_userName");
        }
    } else {
        stAdminResponse.iErrorCode = -6;
        strcpy(stAdminResponse.sErrorDetail, "NO_userName");
    }
    if (cJSON_HasObjectItem(root, "userSex")) {
        int flag = snprintf(userItemInfo.sUserSex, sizeof(userItemInfo.sUserSex), "%s",
                            cJSON_GetObjectItem(root, "userSex")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -7;
            strcpy(stAdminResponse.sErrorDetail, "NO_userSex");
        }
    } else {
        stAdminResponse.iErrorCode = -7;
        strcpy(stAdminResponse.sErrorDetail, "NO_userSex");
    }
    if (cJSON_HasObjectItem(root, "userRemark")) {
        int flag = snprintf(userItemInfo.sUserRemark, sizeof(userItemInfo.sUserRemark), "%s",
                            cJSON_GetObjectItem(root, "userRemark")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -8;
            strcpy(stAdminResponse.sErrorDetail, "NO_userRemark");
        }
    } else {
        stAdminResponse.iErrorCode = -8;
        strcpy(stAdminResponse.sErrorDetail, "NO_userRemark");
    }

    printf("sUserNum:%s,sUserName:%s,sUserSex:%s, sUserRemark:%s\n",
           userItemInfo.sUserNum, userItemInfo.sUserName,userItemInfo.sUserSex,userItemInfo.sUserRemark);

    //组装sql命令
    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum = '%s';",
                 userItemInfo.sUserNum) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询数据库是否存在相同的userNum对应的用户
    db_query(*ppvDBHandle, szSqlBuffer, admin_register_in_callback, &iCbRet);
    //iCbRet为0即userNum对应的用户是存在 返回admin_response_t错误信息 否则添加一个用户
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -4;
        strcpy(stAdminResponse.sErrorDetail, "USER_ALREADY_EXIST");
    }
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        //将插入命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "insert into lib_user (userNum, userName, userSex,userRemark) values ('%s','%s','%s','%s');",
                     userItemInfo.sUserNum, userItemInfo.sUserName, userItemInfo.sUserSex,userItemInfo.sUserRemark) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        //插入用户
    iCbRet = db_insert(*ppvDBHandle,szSqlBuffer);
    //如果iCbRet为0，则插入成功 否则返回admin_response_t错误信息
    if (0 != iCbRet&& -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -8;
        strcpy(stAdminResponse.sErrorDetail, "INSERT_FAILURED");
    }
    //如果iCbRet为0，则插入成功
    if (0 == iCbRet&& -100 == stAdminResponse.iErrorCode) {
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

//用户鉴权业务函数
static void Certify_user_in(cJSON *root, char *pResponse, int iResLen)
{
    //"messageId" : 5002,
    //"userNum" : "卡号"
    user_item_info_t userItemInfo;      //创建一个用户信息结构体
    void **ppvDBHandle = get_handle();  //创建一个数据库句柄
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];  //创建一个数据库命令缓冲区

    int iCbRet = -1;                    //定义一个返回值
    admin_response_t stAdminResponse;   //定义一个返回信息结构体
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail,"LIBRARY_UNKNOWN_ERROR");

    //判断json对象是否还有元素
    if (cJSON_HasObjectItem(root, "userNum")) {
        //将json中adminId打印（复制）到stAdmin.sAdminId
        int flag = snprintf(userItemInfo.sUserNum, sizeof(userItemInfo.sUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -5;
            strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stAdminResponse.iErrorCode = -5;
        strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
    }

    printf("sUserNum:%s\n",userItemInfo.sUserNum);

    //组装sql命令
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum ='%s';",
                 userItemInfo.sUserNum) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /*查询是否有相同的用户*/
    db_query(*ppvDBHandle, szSqlBuffer, admin_login_in_callback, &iCbRet);
    //iCbRet为0，即adminId对应的管理员存在 否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
////////
    }

}


//删除函数的回调函数
static int Delete_user_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData)
{
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
    //打印出查询到的userNum的信息
    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
    for (i = 0; i < iCol; i++) {
        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
    }
    return 0;
}
//删除用户业务函数
static void Delete_user_in(cJSON *root, char *pResponse, int iResLen)
{
    user_item_info_t userItemInfo;                      //创建一个管理员id
    void **ppvDBHandle = get_handle();                  //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];             //创建一个数据库命令缓冲区

    int iCbRet = -1;                                    //定义一个返回值
    admin_response_t stAdminResponse;                   //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    if (cJSON_HasObjectItem(root, "userNum")){
        //将json中userNum打印（复制）到stAdmin.sAdminId
        int flag = snprintf(userItemInfo.sUserNum, sizeof(userItemInfo.sUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stAdminResponse.iErrorCode = -5;
            strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stAdminResponse.iErrorCode = -5;
        strcpy(stAdminResponse.sErrorDetail, "NO_userNum");
    }

    printf("sUserNum:%s\n", userItemInfo.sUserNum);

    //将查询UserNum信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum =\"%s\";",
                 userItemInfo.sUserNum) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    /* 查询数据库是否存在UserNum对应的管理员 */
    db_query(*ppvDBHandle, szSqlBuffer, Delete_user_in_callback, &iCbRet);
    //iCbRet为0即adminId对应的管理员是存在  否则返回admin_response_t错误信息
    if (0 != iCbRet && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {

        //将删除用户命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "delete from lib_user where userNum = \"%s\";", userItemInfo.sUserNum) <=
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
            admin_register_in(root, pResponse, iResLen);
            break;
        case 1004:
            admin_register_in(root, pResponse, iResLen);
            break;
        case 2001:
            display_book_in(root, pResponse, iResLen);
            break;
        case 2002:
            Storage_book_in(root, pResponse, iResLen);
            break;
        case 2003:
            Search_book_in(root, pResponse, iResLen);
            break;
        case 5001:
            Add_user_in(root, pResponse, iResLen);
            break;
        case 5002:
            Certify_user_in(root, pResponse, iResLen);
            break;
        case 5003:
            Delete_user_in(root, pResponse, iResLen);
            break;
        default:
            printf("have no this message:%d\n", nMessageId);
            break;
    }

    cJSON_Delete(root);
    return 0;
}







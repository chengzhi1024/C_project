//
// Created by yang on 2019/6/24.
//

#include "business.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "db_mysql/db_interface.h"
#include <cJSON.h>


/**
 * @brief display_book 回调函数 把每一行的数据按结构体去保存并返还结构体首地址
 * @param
 */
int display_book_callback(char **ppcArgs, int iRow, int iCol, void* pvData)
{
    book_item_info_t *pItems = (book_item_info_t *)pvData;

    if(9 != iCol) return -1;
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

//显示所有书籍的数目回调函数
static int total_book_callback(char **ppcArgs,int iRow, int iCol, void* pvData)
{
    int *iRet = pvData;
    *iRet = atoi(ppcArgs[0]);
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


    //将查询信息打印（复制）到字符串szSqlBuffer
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

/**
 * @brief 显示图书函数，返回一个json对象
 * @param
 * @param
 */
static void display_book(cJSON *root, char *pResponse, int iResLen) {
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

//组装sql查询命令
    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book limit %d,%d", iStart, iEnd);
//查询数据库
    book_item_info_t *pItems = (book_item_info_t *) malloc(sizeof(book_item_info_t)*iEnd);
    db_query(*ppvDBHandle, szSqlBuffer, display_book_callback, (void *) pItems);

//转化为json字符串
    cJSON *json = cJSON_CreateObject();
    cJSON *array = cJSON_CreateObject();

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


    free(pItems);
    cJSON_free(pResJson);
    cJSON_Delete(json);
}

void** get_handle()
{
    static void* pvDBHandle = NULL;
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
            printf("1***\n");
            admin_login_in(root, pResponse, iResLen);
            printf("2***\n");
            break;
        case 1002:
            printf("3***\n");
            admin_register_in(root, pResponse, iResLen);
            printf("4***\n");
            break;
        case 2001:
            display_book(root, pResponse, iResLen);
            break;
        default:
            printf("have no this message:%d\n", nMessageId);
            break;
    }

    cJSON_Delete(root);
    return 0;
}







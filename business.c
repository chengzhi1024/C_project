//
// Created by yang on 2019/6/24.
//

#include "business.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <db_interface.h>
#include <cJSON.h>

static int total_book_callback(char **ppcArgs,int iRow, int iCol, void* pvData)
{

}

/** 登录请求的回调函数*/
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
static void admin_login_in(cJSON *root, char *pResponse, int iResLen)
{
    admin_info_t stAdmin;                           //管理员信息结构体
    void **ppvDBHandle = get_handle();               //拿到一个数据库操作句柄
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];         //sql命令存储

    int iCbRet = -1;                                //回调函数标记
    admin_response_t stAdminResponse;               //返回信息结构体
    stAdminResponse.iErrorCode = -100;              //初始化
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR"); //初始化

    if(cJSON_HasObjectItem(root, "adminId")) {
        strcpy(stAdmin.sAdminId, cJSON_GetObjectItem(root, "adminId")->valuestring);
    }
    if(cJSON_HasObjectItem(root, "adminPasswd")) {
        strcpy(stAdmin.sAdminPasswd, cJSON_GetObjectItem(root, "adminPasswd")->valuestring);
    }
    if(cJSON_HasObjectItem(root, "adminName")) {
        strcpy(stAdmin.sAdminName, cJSON_GetObjectItem(root, "adminName")->valuestring);
    }
    printf("sAdminId:%s,sAdminPasswd:%s,sAdminName:%s\n",
           stAdmin.sAdminId, stAdmin.sAdminPasswd, stAdmin.sAdminName);

    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_admin where adminId=\"%s\";",
             stAdmin.sAdminId);
    /* 查询数据库 */
    db_query(*ppvDBHandle, szSqlBuffer, admin_login_in_callback, (void*)&iCbRet);
    if (0 != iCbRet) {
        stAdminResponse.iErrorCode = -3;
        strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
    }

    if (0 == iCbRet) {
        iCbRet = -1;
        snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_admin where adminId=\"%s\" and adminPasswd=\"%s\";",
                 stAdmin.sAdminId, stAdmin.sAdminPasswd);
        /* 查询数据库 */
        db_query(*ppvDBHandle, szSqlBuffer, admin_login_in_callback, (void*)&iCbRet);
        if (0 != iCbRet) {
            stAdminResponse.iErrorCode = -1;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_PASSWD_ERROR");
        }
        else {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "success");
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


void** get_handle()
{
    static void* pvDBHandle = NULL;
    return &pvDBHandle;
}

int exec_business(const char *pRequest, int iReqLen, char *pResponse, int iResLen)
{
    int iRet;
    int nMessageId;

    // 解析json字符串
    cJSON *root = cJSON_Parse(pRequest);
    if(NULL == root) return -1;

    if(!cJSON_HasObjectItem(root, "messageId")) {
        printf("have no messageId\n");
        return -1;
    }
    nMessageId = cJSON_GetObjectItem(root, "messageId")->valueint;
    switch(nMessageId)
    {
        case 1001:
            admin_login_in(root, pResponse, iResLen);
            break;
        case 1002:

        default:
            printf("have no this message:%d\n", nMessageId);
            break;
    }

    cJSON_Delete(root);

    return 0;
}

/**
 * @brief display_book 回调函数
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
    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book");
//查询数据库，查找所有记录数
//   db_query(*ppvDBHandle, szSqlBuffer,total_book_callback, (void *)&iTotal);

//组装sql查询命令
    snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book limit %d,%d", iStart, iEnd);
//查询数据库
    book_item_info_t *pItems = (book_item_info_t *) malloc(sizeof(book_item_info_t));
    db_query(*ppvDBHandle, szSqlBuffer, display_book_callback, (void *) pItems);

//转化为json字符串
    cJSON *json = cJSON_CreateObject();
    cJSON *array = cJSON_CreateObject();

    for (int (i) = 0; (i) < 9; i++) {
        cJSON *pRowJson = cJSON_CreateObject();
        cJSON_AddStringToObject(pRowJson, "bookId", pItems[i].sbookId);
        cJSON_AddStringToObject(pRowJson, "bookName", pItems[i].sbookName);
        cJSON_AddStringToObject(pRowJson, "bookName", pItems[i].sbookName);
        cJSON_AddNumberToObject(pRowJson, "bookStock", pItems[i].ibookStock);
        cJSON_AddNumberToObject(pRowJson, "bookRemain", pItems[i].ibookRemain);
        cJSON_AddNumberToObject(pRowJson, "bookTimes", pItems[i].ibookTimes);
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




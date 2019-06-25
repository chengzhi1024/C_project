//
// Created by yang on 2019/6/24.
//

#include "business.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <db_interface.h>
#include <cJSON.h>

/** 登录请求 */
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
static void admin_login_in(cJSON *root, char *pResponse, int iResLen)
{
    admin_info_t stAdmin;
    void **ppvDBHandle = get_handle();
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];

    int iCbRet = -1;
    admin_response_t stAdminResponse;
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

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
        default:
            printf("have no this message:%d\n", nMessageId);
            break;
    }

    cJSON_Delete(root);

    return 0;
}


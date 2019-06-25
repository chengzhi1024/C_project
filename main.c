#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "business.h"
#include "Cjson/cJSON.h"

struct request {
    int messageId;
    char adminId[64];
    char *adminPasswd[64];
    char *adminName[64];
} request;

int main() {

    //创建一个cjson结构体指针
    cJSON *root = cJSON_CreateObject();
    //添加键值队
    cJSON_AddNumberToObject(root, "messageId", 1002);
    cJSON_AddStringToObject(root, "adminId", "1");
    cJSON_AddStringToObject(root, "adminPasswd", "123");
    cJSON_AddStringToObject(root, "adminName", "admin");
    char *out = cJSON_PrintUnformatted(root);

    char *pRequest = out;
    int iReqLen=1024;
    char pResponse[1024];
    int iResLen=1024;

    printf("%s\n", pRequest);
    exec_business(pRequest, iReqLen, pResponse, iResLen);
    printf("%s\n", pResponse);

    return 0;
}

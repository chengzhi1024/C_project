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
//    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
//    //打印出查询到的信息
//    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
//    for (i = 0; i < iCol; i++) {
//        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
//    }
    return 0;
}

//1.2管理员注册返回函数
static int admin_register_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
//    //打印出查询到的adminid的信息
//    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
//    for (i = 0; i < 1; i++) {
//        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
//    }
    return 0;
}

//1.3管理员密码修改返回函数
static int admin_modifyPassword_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
//    //打印出查询到的adminid的信息
//    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
//    for (i = 0; i < 2; i++) {
//        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
//    }
    return 0;
}

//1.4管理员注销返回函数
static int admin_logout_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
//    //打印出查询到的adminid的信息
//    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
//    for (i = 0; i < iCol; i++) {
//        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
//    }
    return 0;
}

//2.1显示图书返回函数
//2.1.1显示图书返回书籍的数量函数
static int display_book_in_callback_total(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int *pRet = (int *) pvData;
    *pRet = 0;
    *pRet = atoi(ppcArgs[0]);
    return 0;
}

//2.1.2显示图书返回函数
static int display_book_in_callback_book_item(char **ppcArgs, int iRow, int iCol, void *pvData) {

    book_info_t *pRet;
    pRet = (book_info_t *) pvData + (iRow - 1);

    if (9 != iCol) return -1;
    snprintf(pRet->sbookId, sizeof(pRet->sbookName), "%s", ppcArgs[0]);
    snprintf(pRet->sbookName, sizeof(pRet->sbookId), "%s", ppcArgs[1]);
    snprintf(pRet->sbookAuthor, sizeof(pRet->sbookAuthor), "%s", ppcArgs[2]);
    if (NULL == ppcArgs[3]) {
        pRet->ibookStock = 0;
    } else {
        pRet->ibookStock = atoi(ppcArgs[3]);;
    }
    if (NULL == ppcArgs[4]) {
        pRet->ibookRemain = 0;
    } else {
        pRet->ibookRemain = atoi(ppcArgs[4]);
    }
    if (NULL == ppcArgs[5]) {
        pRet->ibookTimes = 0;
    } else {
        pRet->ibookTimes = atoi(ppcArgs[5]);
    }
    snprintf(pRet->sbookCategory, sizeof(pRet->sbookCategory), "%s", ppcArgs[6]);
    snprintf(pRet->sbookPublisher, sizeof(pRet->sbookPublisher), "%s", ppcArgs[7]);
    snprintf(pRet->sbookPublicationDate, sizeof(pRet->sbookPublicationDate), "%s", ppcArgs[8]);

    return 0;
}

//2.2图书入库返回函数
static int book_storage_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
//    int i;
    int *pRet = (int *) pvData;
    *pRet = 0;

//    //打印出查询到的bookId的信息
//    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
//    for (i = 0; i < 1; i++) {
//        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
//    }
    return 0;
}

//2.3.1查询图书返回书籍的数量函数
static int search_book_in_callback_total(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int *pRet = (int *) pvData;
    *pRet = 0;
    *pRet = atoi(ppcArgs[0]);
    return 0;
}

//2.3.2查询图书返回函数
static int search_book_in_callback_book_item(char **ppcArgs, int iRow, int iCol, void *pvData) {

    book_info_t *pRet;
    pRet = (book_info_t *) pvData + (iRow - 1);

    if (9 != iCol) return -1;
    snprintf(pRet->sbookId, sizeof(pRet->sbookName), "%s", ppcArgs[0]);
    snprintf(pRet->sbookName, sizeof(pRet->sbookId), "%s", ppcArgs[1]);
    snprintf(pRet->sbookAuthor, sizeof(pRet->sbookAuthor), "%s", ppcArgs[2]);
    if (NULL == ppcArgs[3]) {
        pRet->ibookStock = 0;
    } else {
        pRet->ibookStock = atoi(ppcArgs[3]);;
    }
    if (NULL == ppcArgs[4]) {
        pRet->ibookRemain = 0;
    } else {
        pRet->ibookRemain = atoi(ppcArgs[4]);
    }
    if (NULL == ppcArgs[5]) {
        pRet->ibookTimes = 0;
    } else {
        pRet->ibookTimes = atoi(ppcArgs[5]);
    }
    snprintf(pRet->sbookCategory, sizeof(pRet->sbookCategory), "%s", ppcArgs[6]);
    snprintf(pRet->sbookPublisher, sizeof(pRet->sbookPublisher), "%s", ppcArgs[7]);
    snprintf(pRet->sbookPublicationDate, sizeof(pRet->sbookPublicationDate), "%s", ppcArgs[8]);

    return 0;
}

//3.1用户借书返回函数
//3.1.1用户借书返回剩余函数
static int user_borrow_book_in_callback_remain(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;
    *pRet = 0;
    *pRet = atoi(ppcArgs[0]);

    return 0;
}

//3.1.2用户借书返回用户借书所有信息函数
static int user_borrow_book_in_callback_book_item(char **ppcArgs, int iRow, int iCol, void *pvData) {
    book_borrow_callback_info_t *pRet;
    pRet = (book_borrow_callback_info_t *) pvData + (iRow - 1);

    if (8 != iCol) return -1;
    snprintf(pRet->sbookName, sizeof(pRet->sbookName), "%s", ppcArgs[0]);
    snprintf(pRet->sbookId, sizeof(pRet->sbookId), "%s", ppcArgs[1]);
    snprintf(pRet->sbookAuthor, sizeof(pRet->sbookAuthor), "%s", ppcArgs[2]);
    snprintf(pRet->sbookCategory, sizeof(pRet->sbookCategory), "%s", ppcArgs[3]);
    snprintf(pRet->sbookPublisher, sizeof(pRet->sbookPublisher), "%s", ppcArgs[4]);
    snprintf(pRet->sbookPublicationDate, sizeof(pRet->sbookPublicationDate), "%s", ppcArgs[5]);
    snprintf(pRet->sborRetDateLimit, sizeof(pRet->sborRetDateLimit), "%s", ppcArgs[6]);
    snprintf(pRet->sborData, sizeof(pRet->sborData), "%s", ppcArgs[7]);
    //打印出查询到的信息
//    printf("%s ", pRet->sbookName);
//    printf("%s ", pRet->sbookId);
//    printf("%s ", pRet->sbookAuthor);
//    printf("%s ", pRet->sbookCategory);
//    printf("%s ", pRet->sbookPublisher);
//    printf("%s ", pRet->sbookPublicationDate);
//    printf("%s ", pRet->sborRetDateLimit);
//    printf("%s \n", pRet->sborData);

    return 0;
}

//3.1.3用户借书返回借书数量函数
static int user_borrow_book_in_callback_total(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int *pRet = (int *) pvData;
    *pRet = 0;
    *pRet = atoi(ppcArgs[0]);
    return 0;
}

//4.1用户借书返回函数
//4.1.1用户还书返回剩余函数
static int user_return_book_in_callback_ishave(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int *pRet = (int *) pvData;
    *pRet = 0;
    return 0;
}

//4.1.2用户还书返回借阅状态函数
static int user_return_book_in_callback_borrow_redate(char **ppcArgs, int iRow, int iCol, void *pvData) {
    char *pRet = (char *) pvData;
    snprintf(pRet, 24, "%s", ppcArgs[0]);
//    *pRet = 0;
//    if (1 == atoi(ppcArgs[0])) {
//        *pRet = 1;
//    }
    return 0;
}

//4.1.3用户还书返回用户借书所有信息函数
static int user_return_book_in_callback_book_item(char **ppcArgs, int iRow, int iCol, void *pvData) {
    book_retrun_callback_info_t *pRet;
    pRet = (book_retrun_callback_info_t *) pvData + (iRow - 1);

    if (9 != iCol) return -1;
    snprintf(pRet->sbookName, sizeof(pRet->sbookName), "%s", ppcArgs[0]);
    snprintf(pRet->sbookId, sizeof(pRet->sbookId), "%s", ppcArgs[1]);
    snprintf(pRet->sbookAuthor, sizeof(pRet->sbookAuthor), "%s", ppcArgs[2]);
    snprintf(pRet->sbookCategory, sizeof(pRet->sbookCategory), "%s", ppcArgs[3]);
    snprintf(pRet->sbookPublisher, sizeof(pRet->sbookPublisher), "%s", ppcArgs[4]);
    snprintf(pRet->sbookPublicationDate, sizeof(pRet->sbookPublicationDate), "%s", ppcArgs[5]);
    snprintf(pRet->sborRetDateLimit, sizeof(pRet->sborRetDateLimit), "%s", ppcArgs[6]);
    snprintf(pRet->sborData, sizeof(pRet->sborData), "%s", ppcArgs[7]);
    snprintf(pRet->sretDate, sizeof(pRet->sretDate), "%s", ppcArgs[8]);
    //打印出查询到的信息
//    printf("%s ", pRet->sbookName);
//    printf("%s ", pRet->sbookId);
//    printf("%s ", pRet->sbookAuthor);
//    printf("%s ", pRet->sbookCategory);
//    printf("%s ", pRet->sbookPublisher);
//    printf("%s ", pRet->sbookPublicationDate);
//    printf("%s ", pRet->sborRetDateLimit);
//    printf("%s \n", pRet->sborData);

    return 0;
}

//4.1.4用户还书返回借书数量函数
static int user_return_book_in_callback_total(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int *pRet = (int *) pvData;
    *pRet = 0;
    *pRet = atoi(ppcArgs[0]);
    return 0;
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

//    //打印出查询到的信息
//    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
//    for (i = 1; i < iCol; i++) {
//        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
//    }
    return 0;
}

//5.3删除用户返回函数
static int delete_user_in_callback(char **ppcArgs, int iRow, int iCol, void *pvData) {
    int i;
    int *pRet = (int *) pvData;

    *pRet = 0;
//    //打印出查询到的adminid的信息
//    printf("ppcArgs:%p, iRow:%d, iCol:%d\n", ppcArgs, iRow, iCol);
//    for (i = 0; i < iCol; i++) {
//        printf("row[%d] col[%d] value [%s]\n", iRow, i, ppcArgs[i]);
//    }
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
        strcpy(stAdminResponse.sErrorDetail, "NO_USER_OR_PASSWD_ERROR");
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

//2.1显示图书函数
static void display_book_in(cJSON *root, char *pResponse, int iResLen) {

    book_display_position_t stBookDisplay;       //创建一个图书显示位置
    void **ppvDBHandle = get_handle();          //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE];     //创建一个数据库命令缓冲区
    book_info_t *pCbRet = NULL;                   //定义一个返回书籍条目结构体指针
    int iCbRet = -1;
    int number = 0;
    book_display_response_t stBookDisplayResponse;//创建一个图书显示返回结构体并赋初值
    stBookDisplayResponse.iErrorCode = -100;
    strcpy(stBookDisplayResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
    stBookDisplayResponse.itotal = 0;           //初始化返回书籍条目数量

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给借书信息结构体
    if (cJSON_HasObjectItem(root, "start")) {
        //将json中start赋值给stBookDisplay.iStart
        stBookDisplay.iStart = cJSON_GetObjectItem(root, "start")->valueint;
    } else {
        stBookDisplayResponse.iErrorCode = -28;
        strcpy(stBookDisplayResponse.sErrorDetail, "NO_start");
    }
    if (cJSON_HasObjectItem(root, "end")) {
        //将json中end赋值给stBookDisplay.iEnd
        stBookDisplay.iEnd = cJSON_GetObjectItem(root, "end")->valueint;
    } else {
        stBookDisplayResponse.iErrorCode = -29;
        strcpy(stBookDisplayResponse.sErrorDetail, "NO_end");
    }
    printf("iStart:%d,iEnd:%d\n", stBookDisplay.iStart, stBookDisplay.iEnd);

    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book;") <= 0) {
        stBookDisplayResponse.iErrorCode = -101;
        strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询数据库 查找所有记录数
    int flag = db_query(*ppvDBHandle, szSqlBuffer, display_book_in_callback_total, &iCbRet);
    if (0 != flag && -100 == stBookDisplayResponse.iErrorCode) {
        stBookDisplayResponse.iErrorCode = -24;
        strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
    }
    if (0 == flag && -100 == stBookDisplayResponse.iErrorCode) {
        stBookDisplayResponse.itotal = iCbRet;
        if (stBookDisplayResponse.itotal <= (stBookDisplay.iEnd - stBookDisplay.iStart)) {
            number = stBookDisplayResponse.itotal;
        } else {
            if (stBookDisplayResponse.itotal >= stBookDisplay.iEnd) {
                number = stBookDisplay.iEnd - stBookDisplay.iStart;
            } else {
                if (stBookDisplayResponse.itotal <= stBookDisplay.iStart) {
                    number = 0;
                } else {
                    number = stBookDisplayResponse.itotal - stBookDisplay.iStart;
                }
            }
        }
        iCbRet = -1;
        //将查询信息打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book limit %d,%d;", stBookDisplay.iStart,
                     number) <= 0) {
            stBookDisplayResponse.iErrorCode = -101;
            strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
        }
        //查询数据库
        book_info_t *pItems = (book_info_t *) malloc(sizeof(book_info_t) * number);
        pCbRet = pItems;
        iCbRet = db_query(*ppvDBHandle, szSqlBuffer, display_book_in_callback_book_item, pItems);
        if (0 != iCbRet && -100 == stBookDisplayResponse.iErrorCode) {
            stBookDisplayResponse.iErrorCode = -24;
            strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
        }
        if (0 == iCbRet && -100 == stBookDisplayResponse.iErrorCode) {
            stBookDisplayResponse.iErrorCode = 0;
            strcpy(stBookDisplayResponse.sErrorDetail, "LIBRARY_OK");
        }
    }

    //转化为json字符串
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON *bookarr = cJSON_CreateArray();
    if (!bookarr) return;
    cJSON_AddNumberToObject(json, "messageId", 2001);
    cJSON_AddNumberToObject(json, "errorCode", stBookDisplayResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stBookDisplayResponse.sErrorDetail);
    cJSON_AddNumberToObject(json, "total", stBookDisplayResponse.itotal);

    for (int i = 0; i < number; i++) {
        cJSON *bookitem = cJSON_CreateObject();
        cJSON_AddStringToObject(bookitem, "bookId", (pCbRet + i)->sbookId);
        cJSON_AddStringToObject(bookitem, "bookName", (pCbRet + i)->sbookName);
        cJSON_AddStringToObject(bookitem, "bookAuthor", (pCbRet + i)->sbookAuthor);
        cJSON_AddNumberToObject(bookitem, "bookStock", (pCbRet + i)->ibookStock);
        cJSON_AddNumberToObject(bookitem, "bookRemain", (pCbRet + i)->ibookRemain);
        cJSON_AddNumberToObject(bookitem, "bookTimes", (pCbRet + i)->ibookTimes);
        cJSON_AddStringToObject(bookitem, "bookCategory", (pCbRet + i)->sbookCategory);
        cJSON_AddStringToObject(bookitem, "bookPublisher", (pCbRet + i)->sbookPublisher);
        cJSON_AddStringToObject(bookitem, "bookPublicationDate", (pCbRet + i)->sbookPublicationDate);
        cJSON_AddItemToArray(bookarr, bookitem);
    }
    cJSON_AddItemToObject(json, "item", bookarr);

    char *pResJson = cJSON_Print(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
    free(pCbRet);
}

//2.2图书入库函数
static void book_storage_in(cJSON *root, char *pResponse, int iResLen) {

    book_info_t stBookInfo;                   //创建一个图书信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库句柄
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区

    int iCbRet = -1;                        //定义一个返回值
    admin_response_t stAdminResponse;       //创建一个返回信息结构体 并赋初值
    stAdminResponse.iErrorCode = -100;
    strcpy(stAdminResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给管理员结构体
    if (cJSON_HasObjectItem(root, "bookId")) {
        int flag = snprintf(stBookInfo.sbookId, sizeof(stBookInfo.sbookId), "%s",
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
        int flag = snprintf(stBookInfo.sbookName, sizeof(stBookInfo.sbookName), "%s",
                            cJSON_GetObjectItem(root, "bookName")->valuestring);
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
        int flag = snprintf(stBookInfo.sbookAuthor, sizeof(stBookInfo.sbookAuthor), "%s",
                            cJSON_GetObjectItem(root, "bookAuthor")->valuestring);
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
        stBookInfo.ibookStock = cJSON_GetObjectItem(root, "bookAddNumber")->valueint;
        if (stBookInfo.ibookStock < 0) {
            stAdminResponse.iErrorCode = -35;
            strcpy(stAdminResponse.sErrorDetail, "NUMBER_EORROR");
        }
    } else {
        stAdminResponse.iErrorCode = -29;
        strcpy(stAdminResponse.sErrorDetail, "NO_bookAddNumber");
    }


    if (cJSON_HasObjectItem(root, "bookCategory")) {
        int flag = snprintf(stBookInfo.sbookCategory, sizeof(stBookInfo.sbookCategory), "%s",
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
        int flag = snprintf(stBookInfo.sbookPublisher, sizeof(stBookInfo.sbookPublisher), "%s",
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
        int flag = snprintf(stBookInfo.sbookPublicationDate, sizeof(stBookInfo.sbookPublicationDate), "%s",
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
           stBookInfo.sbookId, stBookInfo.sbookName, stBookInfo.sbookAuthor, stBookInfo.ibookStock,
           stBookInfo.sbookCategory, stBookInfo.sbookPublisher, stBookInfo.sbookPublicationDate);


    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookId= '%s';",
                 stBookInfo.sbookId) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    // 查询数据库是否存在bookId对应的图书
    db_query(*ppvDBHandle, szSqlBuffer, book_storage_in_callback, &iCbRet);
    //iCbRet为0即bookId对应的图书是存在 返回admin_response_t错误信息 否则添加一个管理员
    if (0 == iCbRet && -100 == stAdminResponse.iErrorCode) {
        iCbRet = -1;
        //将更新命令打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                     "update lib_book set bookStock = bookStock + %d,bookRemain = bookRemain + %d where bookId = '%s';",
                     stBookInfo.ibookStock, stBookInfo.ibookStock, stBookInfo.sbookId) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
        }
        // 更新图书库存
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
//        //得到当前归还时间
//        time_t timep;
//        struct tm *p;
//        time(&timep);
//        p = gmtime(&timep);
//        char curRetTime[32];
//        if (snprintf(curRetTime, sizeof(curRetTime), "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", 1900 + p->tm_year,
//                     1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec) <= 0) {
//            stAdminResponse.iErrorCode = -101;
//            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
//        }
//        printf("time:%s\n", curRetTime);
        //将查询信息打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                     "insert into lib_book (bookId,bookName,bookAuthor,bookStock,bookRemain,bookCategory,bookPublisher,bookPublicationDate) values ('%s','%s','%s','%d','%d','%s','%s','%s');",
                     stBookInfo.sbookId, stBookInfo.sbookName, stBookInfo.sbookAuthor, stBookInfo.ibookStock,
                     stBookInfo.ibookStock,
                     stBookInfo.sbookCategory, stBookInfo.sbookPublisher, stBookInfo.sbookPublicationDate) <= 0) {
            stAdminResponse.iErrorCode = -101;
            strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");

        }
        // 插入图书
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

    // 组装数据库为json字符串
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

//2.3查询图书函数
static void search_book_in(cJSON *root, char *pResponse, int iResLen) {

    book_search_info_t stBookSearch;        //创建一个图书搜索信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
    int iCbRet = -1;                        //定义一个返回值
    book_info_t *pCbRet = NULL;             //定义一个返回书籍条目结构体指针
    book_display_response_t stBookDisplayResponse;//创建一个图书显示返回结构体并赋初值
    stBookDisplayResponse.iErrorCode = -100;
    strcpy(stBookDisplayResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
    stBookDisplayResponse.itotal = 0;

    //解析json对象
    if (cJSON_HasObjectItem(root, "bookSearch")) {
        int flag = snprintf(stBookSearch.sBookSearch, sizeof(stBookSearch.sBookSearch), "%s",
                            cJSON_GetObjectItem(root, "bookSearch")->valuestring);
        if (flag < 0) {
            stBookDisplayResponse.iErrorCode = -101;
            strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stBookDisplayResponse.iErrorCode = -30;
            strcpy(stBookDisplayResponse.sErrorDetail, "NO_bookSearch");
        }
    } else {
        stBookDisplayResponse.iErrorCode = -30;
        strcpy(stBookDisplayResponse.sErrorDetail, "NO_bookSearch");
    }
    if (cJSON_HasObjectItem(root, "type")) {
        stBookSearch.iType = cJSON_GetObjectItem(root, "type")->valueint;
    } else {
        stBookDisplayResponse.iErrorCode = -31;
        strcpy(stBookDisplayResponse.sErrorDetail, "NO_type");
    }

    //打印数据测试是否解析成功
    printf("bookSearch:%s,iType: %d\n", stBookSearch.sBookSearch, stBookSearch.iType);

    //判断查询类型 一共有三种类型，0代表书号，1代表书名，2代表作者
    if (-100 == stBookDisplayResponse.iErrorCode) {
        int flag = -1;
        switch (stBookSearch.iType) {
            case 0:
                //将查询信息打印（复制）到字符串szSqlBuffer
                if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book where bookId = '%s';",
                             stBookSearch.sBookSearch) <= 0) {
                    stBookDisplayResponse.iErrorCode = -101;
                    strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
                }
                //查询数据库，查找所有记录数
                flag = db_query(*ppvDBHandle, szSqlBuffer, search_book_in_callback_total, &iCbRet);
                if (0 != flag && -100 == stBookDisplayResponse.iErrorCode) {
                    stBookDisplayResponse.iErrorCode = -24;
                    strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
                }
                if (0 == flag && -100 == stBookDisplayResponse.iErrorCode) {
                    stBookDisplayResponse.itotal = iCbRet;
                    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookId = '%s';",
                                 stBookSearch.sBookSearch) <= 0) {
                        stBookDisplayResponse.iErrorCode = -101;
                        strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
                    }
                    flag = -1;
                    //查询数据库
                    book_info_t *stCbRet = (book_info_t *) malloc(sizeof(book_info_t) * stBookDisplayResponse.itotal);
                    pCbRet = stCbRet;
                    flag = db_query(*ppvDBHandle, szSqlBuffer, search_book_in_callback_book_item, stCbRet);
                    if (0 != flag && -100 == stBookDisplayResponse.iErrorCode) {
                        stBookDisplayResponse.iErrorCode = -24;
                        strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
                    }
                    if (0 == flag && -100 == stBookDisplayResponse.iErrorCode) {
                        stBookDisplayResponse.iErrorCode = 0;
                        strcpy(stBookDisplayResponse.sErrorDetail, "LIBRARY_0k");
                    }
                }
                break;
            case 1:
                //将查询信息打印（复制）到字符串szSqlBuffer
                if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book where bookName = '%s';",
                             stBookSearch.sBookSearch) <= 0) {
                    stBookDisplayResponse.iErrorCode = -101;
                    strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
                }
                //查询数据库，查找所有记录数
                flag = db_query(*ppvDBHandle, szSqlBuffer, search_book_in_callback_total, &iCbRet);
                if (0 != flag && -100 == stBookDisplayResponse.iErrorCode) {
                    stBookDisplayResponse.iErrorCode = -24;
                    strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
                }
                if (0 == flag && -100 == stBookDisplayResponse.iErrorCode) {
                    stBookDisplayResponse.itotal = iCbRet;
                    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookName = '%s';",
                                 stBookSearch.sBookSearch) <= 0) {
                        stBookDisplayResponse.iErrorCode = -101;
                        strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
                    }
                    flag = -1;
                    //查询数据库
                    book_info_t *stCbRet = (book_info_t *) malloc(sizeof(book_info_t) * stBookDisplayResponse.itotal);
                    pCbRet = stCbRet;
                    flag = db_query(*ppvDBHandle, szSqlBuffer, search_book_in_callback_book_item, stCbRet);
                    if (0 != flag && -100 == stBookDisplayResponse.iErrorCode) {
                        stBookDisplayResponse.iErrorCode = -24;
                        strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
                    }
                    if (0 == flag && -100 == stBookDisplayResponse.iErrorCode) {
                        stBookDisplayResponse.iErrorCode = 0;
                        strcpy(stBookDisplayResponse.sErrorDetail, "LIBRARY_0k");
                    }
                }
                break;
            case 2:
                //将查询信息打印（复制）到字符串szSqlBuffer
                if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select count(*) from lib_book where bookAuthor = '%s';",
                             stBookSearch.sBookSearch) <= 0) {
                    stBookDisplayResponse.iErrorCode = -101;
                    strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
                }
                //查询数据库，查找所有记录数
                flag = db_query(*ppvDBHandle, szSqlBuffer, search_book_in_callback_total, &iCbRet);
                if (0 != flag && -100 == stBookDisplayResponse.iErrorCode) {
                    stBookDisplayResponse.iErrorCode = -24;
                    strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
                }
                if (0 == flag && -100 == stBookDisplayResponse.iErrorCode) {
                    stBookDisplayResponse.itotal = iCbRet;
                    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookAuthor = '%s';",
                                 stBookSearch.sBookSearch) <= 0) {
                        stBookDisplayResponse.iErrorCode = -101;
                        strcpy(stBookDisplayResponse.sErrorDetail, "snprintf_ERROR");
                    }
                    flag = -1;
                    //查询数据库
                    book_info_t *stCbRet = (book_info_t *) malloc(sizeof(book_info_t) * stBookDisplayResponse.itotal);
                    pCbRet = stCbRet;
                    flag = db_query(*ppvDBHandle, szSqlBuffer, search_book_in_callback_book_item, stCbRet);
                    if (0 != flag && -100 == stBookDisplayResponse.iErrorCode) {
                        stBookDisplayResponse.iErrorCode = -24;
                        strcpy(stBookDisplayResponse.sErrorDetail, "QUERY_FAILURED");
                    }
                    if (0 == flag && -100 == stBookDisplayResponse.iErrorCode) {
                        stBookDisplayResponse.iErrorCode = 0;
                        strcpy(stBookDisplayResponse.sErrorDetail, "LIBRARY_0k");
                    }
                }
                break;
            default:
                stBookDisplayResponse.iErrorCode = -32;
                strcpy(stBookDisplayResponse.sErrorDetail, "SEARCH_FAILURED");
                break;
        }
    }

    //转化为json字符串
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON *bookarr = cJSON_CreateArray();
    if (!bookarr) return;
    cJSON_AddNumberToObject(json, "messageId", 2003);
    cJSON_AddNumberToObject(json, "errorCode", stBookDisplayResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stBookDisplayResponse.sErrorDetail);
    cJSON_AddNumberToObject(json, "total", stBookDisplayResponse.itotal);

    for (int i = 0; i < stBookDisplayResponse.itotal; i++) {
        cJSON *bookitem = cJSON_CreateObject();
        cJSON_AddStringToObject(bookitem, "bookId", (pCbRet + i)->sbookId);
        cJSON_AddStringToObject(bookitem, "bookName", (pCbRet + i)->sbookName);
        cJSON_AddStringToObject(bookitem, "bookAuthor", (pCbRet + i)->sbookAuthor);
        cJSON_AddNumberToObject(bookitem, "bookStock", (pCbRet + i)->ibookStock);
        cJSON_AddNumberToObject(bookitem, "bookRemain", (pCbRet + i)->ibookRemain);
        cJSON_AddNumberToObject(bookitem, "bookTimes", (pCbRet + i)->ibookTimes);
        cJSON_AddStringToObject(bookitem, "bookCategory", (pCbRet + i)->sbookCategory);
        cJSON_AddStringToObject(bookitem, "bookPublisher", (pCbRet + i)->sbookPublisher);
        cJSON_AddStringToObject(bookitem, "bookPublicationDate", (pCbRet + i)->sbookPublicationDate);
        cJSON_AddItemToArray(bookarr, bookitem);
    }
    cJSON_AddItemToObject(json, "item", bookarr);

    char *pResJson = cJSON_Print(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
    free(pCbRet);
}

//3.1用户借书函数
static void user_borrow_book_in(cJSON *root, char *pResponse, int iResLen) {

    borrowORreturn_book_info_t stBorrowBook; //创建一个借书信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
    book_borrow_callback_info_t *pCbRet = NULL;     //定义一个返回书籍条目结构体指针
    int iCbRet = -1;                         //定义一个返回值
    borrowORreturn_book_response_t stBorrowBookResponse;       //创建一个返回信息结构体 并赋初值
    stBorrowBookResponse.iErrorCode = -100;
    strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
    stBorrowBookResponse.itotal = 0;           //初始化返回书籍条目数量
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
    if (cJSON_HasObjectItem(root, "bookId")) {
        int flag = snprintf(stBorrowBook.sbookId, sizeof(stBorrowBook.sbookId), "%s",
                            cJSON_GetObjectItem(root, "bookId")->valuestring);
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
        int iCBAuthentication = -1;
        //查询数据库是否存在usertNum对应的用户
        int flag = db_query(*ppvDBHandle, szSqlBuffer, user_authentication_in_callback, &iCBAuthentication);
        //iCbRet为0，即UserNum对应的用户存在 否则返回borrowORreturn_book_response_t错误信息
        if (0 != flag && -100 == stBorrowBookResponse.iErrorCode) {
            stBorrowBookResponse.iErrorCode = -24;
            strcpy(stBorrowBookResponse.sErrorDetail, "QUERY_FAILURED");
        }
        if (0 == flag && -100 == stBorrowBookResponse.iErrorCode) {
            if (0 == iCBAuthentication && -100 == stBorrowBookResponse.iErrorCode) {
                //用户存在 判断剩余
                if (0 < iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
                    //剩余充足 可以借书
                    iCbRet = -1;
                    //将修改信息打印（复制）到字符串szSqlBuffer
                    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                 "update lib_book set lib_book.bookRemain = lib_book.bookRemain - 1 ,lib_book.bookTimes = lib_book.bookTimes + 1 where bookId = \"%s\";",
                                 stBorrowBook.sbookId) <= 0) {
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
                        iCbRet = -1;
                        //得到当前时间和归还时间
                        time_t timep;
                        struct tm *p;
                        time(&timep);
                        p = gmtime(&timep);
                        char curtime[24];
                        char rettime[24];
                        if (snprintf(curtime, sizeof(curtime), "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", 1900 + p->tm_year,
                                     1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec) <= 0) {
                            stBorrowBookResponse.iErrorCode = -101;
                            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                        }
                        if (snprintf(rettime, sizeof(rettime), "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", 1900 + p->tm_year,
                                     3 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec) <= 0) {
                            stBorrowBookResponse.iErrorCode = -101;
                            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                        }
//                    printf("time:%s\n",curtime);
//                    printf("time:%s\n",rettime);

                        //将修改信息打印（复制）到字符串szSqlBuffer
                        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                     "insert into lib_book_borrow (userId,bookId,borStatus,borDate,borRetDateLimit,retDate) values ((select lib_user.userId from lib_user where userNum = \"%s\"),\"%s\",'1',\"%s\",\"%s\",NULL);",
                                     stBorrowBook.sUserNum, stBorrowBook.sbookId, curtime, rettime) <= 0) {
                            stBorrowBookResponse.iErrorCode = -101;
                            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                        }
                        //插入借阅信息
                        iCbRet = db_insert(*ppvDBHandle, szSqlBuffer);
                        if (0 != iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
                            stBorrowBookResponse.iErrorCode = -8;
                            strcpy(stBorrowBookResponse.sErrorDetail, "INSERT_FAILURED");
                        }
                        if (0 == iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
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

                //查询当前用户是否存在借阅信息
                flag = -1;
                //将查询信息打印（复制）到字符串szSqlBuffer
                if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                             "select count(*) from lib_book_borrow where userId in (select userId from lib_user where userNum = \"%s\");",
                             stBorrowBook.sUserNum) <= 0) {
                    stBorrowBookResponse.iErrorCode = -101;
                    strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                }
                //查询数据库是否存在usertNum对应的用户借阅信息
                flag = db_query(*ppvDBHandle, szSqlBuffer, user_borrow_book_in_callback_total, &iCbRet);
                if (0 != flag && (0 == stBorrowBookResponse.iErrorCode || -23 == stBorrowBookResponse.iErrorCode)) {
                    stBorrowBookResponse.iErrorCode = -24;
                    strcpy(stBorrowBookResponse.sErrorDetail, "QUERY_FAILURED");
                } else {
                    stBorrowBookResponse.itotal = iCbRet;
                    if (stBorrowBookResponse.itotal == 0 &&
                        (0 == stBorrowBookResponse.iErrorCode || -23 == stBorrowBookResponse.iErrorCode)) {
                        stBorrowBookResponse.iErrorCode = -25;
                        strcpy(stBorrowBookResponse.sErrorDetail, "NO_BORROW_INFO");
                    }
                    if (stBorrowBookResponse.itotal > 0) {
                        //查询借书信息
                        //定义一个返回书籍条目信息结构体
                        book_borrow_callback_info_t *sCbRet = (book_borrow_callback_info_t *) malloc(
                                sizeof(book_borrow_callback_info_t) * stBorrowBookResponse.itotal);
                        pCbRet = sCbRet;
                        //借书完成 打印当前用户的借阅信息
                        //将查询信息打印（复制）到字符串szSqlBuffer
                        flag = -1;
                        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                     "select bookName,userNum,bookAuthor,bookCategory,bookPublisher,bookPublicationDate,borRetDateLimit,borDate from (lib_book_borrow left join lib_book on lib_book_borrow.bookId = lib_book.bookId) left join lib_user on lib_book_borrow.userId = lib_user.userId where  lib_user.userNum =\"%s\";",
                                     stBorrowBook.sUserNum) <= 0) {
                            stBorrowBookResponse.iErrorCode = -101;
                            strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                        }
                        //查询数据库是否存在usertNum对应的用户借阅信息
                        flag = db_query(*ppvDBHandle, szSqlBuffer, user_borrow_book_in_callback_book_item, sCbRet);
                        if (0 != flag && -100 == stBorrowBookResponse.iErrorCode) {
                            stBorrowBookResponse.iErrorCode = -24;
                            strcpy(stBorrowBookResponse.sErrorDetail, "QUERY_FAILURED");
                        }
                    }
                }
            } else {
                stBorrowBookResponse.iErrorCode = -3;
                strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_NO_USER");
            }
        }
    }

    /* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON *bookarr = cJSON_CreateArray();
    if (!bookarr) return;
    cJSON_AddNumberToObject(json, "messageId", 3001);
    cJSON_AddNumberToObject(json, "errorCode", stBorrowBookResponse.iErrorCode);
    cJSON_AddStringToObject(json, "errorDetail", stBorrowBookResponse.sErrorDetail);
    cJSON_AddNumberToObject(json, "total", stBorrowBookResponse.itotal);

    for (int i = 0; i < stBorrowBookResponse.itotal; i++) {
        cJSON *bookitem = cJSON_CreateObject();
        cJSON_AddStringToObject(bookitem, "bookName", (pCbRet + i)->sbookName);
        cJSON_AddStringToObject(bookitem, "bookNum", (pCbRet + i)->sbookId);
        cJSON_AddStringToObject(bookitem, "bookAuthor", (pCbRet + i)->sbookAuthor);
        cJSON_AddStringToObject(bookitem, "bookCategory", (pCbRet + i)->sbookCategory);
        cJSON_AddStringToObject(bookitem, "bookPublisher", (pCbRet + i)->sbookPublisher);
        cJSON_AddStringToObject(bookitem, "bookPublicationDate", (pCbRet + i)->sbookPublicationDate);
        cJSON_AddStringToObject(bookitem, "borRetDateLimit", (pCbRet + i)->sborRetDateLimit);
        cJSON_AddStringToObject(bookitem, "borDate", (pCbRet + i)->sborData);
        cJSON_AddItemToArray(bookarr, bookitem);
    }
    cJSON_AddItemToObject(json, "item", bookarr);

    char *pResJson = cJSON_Print(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
    free(pCbRet);
}

//3.2用户查询借借书信息函数
static void user_search_borrow_info_book_in(cJSON *root, char *pResponse, int iResLen) {

    borrowORreturn_book_info_t stBorrowBook; //创建一个借书信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
    book_borrow_callback_info_t *pCbRet = NULL;     //定义一个返回书籍条目结构体指针
    int iCbRet = -1;                         //定义一个返回值
    borrowORreturn_book_response_t stBorrowBookResponse;       //创建一个返回信息结构体 并赋初值
    stBorrowBookResponse.iErrorCode = -100;
    strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
    stBorrowBookResponse.itotal = 0;           //初始化返回书籍条目数量
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


    //判断用户是否存在
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
        stBorrowBookResponse.iErrorCode = -24;
        strcpy(stBorrowBookResponse.sErrorDetail, "QUERY_FAILURED");
    }
    if (0 == flag && -100 == stBorrowBookResponse.iErrorCode) {
        if (0 == iCbRet && -100 == stBorrowBookResponse.iErrorCode) {
            //用户存在
            //查询当前用户是否存在借书信息
            iCbRet = -1;
            flag = -1;
            //将查询信息打印（复制）到字符串szSqlBuffer
            if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                         "select count(*) from lib_book_borrow where userId in (select userId from lib_user where userNum = \"%s\" ) and borStatus = 1 ;",
                         stBorrowBook.sUserNum) <= 0) {
                stBorrowBookResponse.iErrorCode = -101;
                strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
            }
            //查询数据库是否存在usertNum对应的用户借阅信息
            flag = db_query(*ppvDBHandle, szSqlBuffer, user_borrow_book_in_callback_total, &iCbRet);
            if (0 != flag && -100 == stBorrowBookResponse.iErrorCode) {
                stBorrowBookResponse.iErrorCode = -24;
                strcpy(stBorrowBookResponse.sErrorDetail, "QUERY_FAILURED");
            }
            if (0 == flag && -100 == stBorrowBookResponse.iErrorCode) {
                stBorrowBookResponse.itotal = iCbRet;
                if (stBorrowBookResponse.itotal == 0) {
                    stBorrowBookResponse.iErrorCode = -25;
                    strcpy(stBorrowBookResponse.sErrorDetail, "NO_BORROW_INFO");
                }
                if (stBorrowBookResponse.itotal > 0) {
                    //查询借书信息
                    //定义一个返回书籍条目信息结构体
                    book_borrow_callback_info_t *sCbRet = (book_borrow_callback_info_t *) malloc(
                            sizeof(book_borrow_callback_info_t) * stBorrowBookResponse.itotal);
                    pCbRet = sCbRet;
                    //借书完成 打印当前用户的借阅信息
                    //将查询信息打印（复制）到字符串szSqlBuffer
                    flag = -1;
                    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                 "select bookName,lib_book.bookId,bookAuthor,bookCategory,bookPublisher,bookPublicationDate,borRetDateLimit,borDate from (lib_book_borrow left join lib_book on lib_book_borrow.bookId = lib_book.bookId) left join lib_user on lib_book_borrow.userId = lib_user.userId where  lib_user.userNum =\"%s\" and borStatus = 1;",
                                 stBorrowBook.sUserNum) <= 0) {
                        stBorrowBookResponse.iErrorCode = -101;
                        strcpy(stBorrowBookResponse.sErrorDetail, "snprintf_ERROR");
                    }
                    //查询数据库是否存在usertNum对应的用户借阅信息
                    flag = db_query(*ppvDBHandle, szSqlBuffer, user_borrow_book_in_callback_book_item, sCbRet);
                    if (0 != flag && -100 == stBorrowBookResponse.iErrorCode) {
                        stBorrowBookResponse.iErrorCode = -24;
                        strcpy(stBorrowBookResponse.sErrorDetail, "QUERY_FAILURED");
                    }
                    if (0 == flag && -100 == stBorrowBookResponse.iErrorCode) {
                        stBorrowBookResponse.iErrorCode = 0;
                        strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_OK");
                    }
                }
            }
        } else {
            stBorrowBookResponse.iErrorCode = -3;
            strcpy(stBorrowBookResponse.sErrorDetail, "LIBRARY_NO_USER");
        }
    }


/* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON *bookarr = cJSON_CreateArray();
    if (!bookarr) return;
    cJSON_AddNumberToObject(json,
                            "messageId", 3002);
    cJSON_AddNumberToObject(json,
                            "errorCode", stBorrowBookResponse.iErrorCode);
    cJSON_AddStringToObject(json,
                            "errorDetail", stBorrowBookResponse.sErrorDetail);
    cJSON_AddNumberToObject(json,
                            "total", stBorrowBookResponse.itotal);

    for (
            int i = 0;
            i < stBorrowBookResponse.
                    itotal;
            i++) {
        cJSON *bookitem = cJSON_CreateObject();
        cJSON_AddStringToObject(bookitem,
                                "bookName", (pCbRet + i)->sbookName);
        cJSON_AddStringToObject(bookitem,
                                "bookId", (pCbRet + i)->sbookId);
        cJSON_AddStringToObject(bookitem,
                                "bookAuthor", (pCbRet + i)->sbookAuthor);
        cJSON_AddStringToObject(bookitem,
                                "bookCategory", (pCbRet + i)->sbookCategory);
        cJSON_AddStringToObject(bookitem,
                                "bookPublisher", (pCbRet + i)->sbookPublisher);
        cJSON_AddStringToObject(bookitem,
                                "bookPublicationDate", (pCbRet + i)->sbookPublicationDate);
        cJSON_AddStringToObject(bookitem,
                                "borRetDateLimit", (pCbRet + i)->sborRetDateLimit);
        cJSON_AddStringToObject(bookitem,
                                "borDate", (pCbRet + i)->sborData);
        cJSON_AddItemToArray(bookarr, bookitem
        );
    }
    cJSON_AddItemToObject(json,
                          "item", bookarr);

    char *pResJson = cJSON_Print(json);
    snprintf(pResponse, iResLen,
             "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
    free(pCbRet);
}


//4.1用户还书函数
static void user_return_book_in(cJSON *root, char *pResponse, int iResLen) {

    borrowORreturn_book_info_t stReturnBook; //创建一个还书信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
    book_retrun_callback_info_t *pCbRet = NULL;     //定义一个返回书籍条目结构体指针
    int iCbRet = -1;                         //定义一个返回值
    borrowORreturn_book_response_t stReturnBookResponse;       //创建一个返回信息结构体 并赋初值
    char sRetborDate[24] = {};                                      //创建一个返回时间数组
    char *pRetborDate = sRetborDate;                                      //创建一个返回时间数组
    stReturnBookResponse.iErrorCode = -100;
    strcpy(stReturnBookResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
    stReturnBookResponse.itotal = 0;               //初始化返回书籍条目数量

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给还书信息结构体
    if (cJSON_HasObjectItem(root, "userNum")) {
        //将json中userNum打印（复制）到stBorrowBook.sUserNum
        int flag = snprintf(stReturnBook.sUserNum, sizeof(stReturnBook.sUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stReturnBookResponse.iErrorCode = -101;
            strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stReturnBookResponse.iErrorCode = -13;
            strcpy(stReturnBookResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stReturnBookResponse.iErrorCode = -13;
        strcpy(stReturnBookResponse.sErrorDetail, "NO_userNum");
    }
    if (cJSON_HasObjectItem(root, "bookId")) {
        int flag = snprintf(stReturnBook.sbookId, sizeof(stReturnBook.sbookId), "%s",
                            cJSON_GetObjectItem(root, "bookId")->valuestring);
        if (flag < 0) {
            stReturnBookResponse.iErrorCode = -101;
            strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stReturnBookResponse.iErrorCode = -21;
            strcpy(stReturnBookResponse.sErrorDetail, "NO_bookId");
        }
    } else {
        stReturnBookResponse.iErrorCode = -21;
        strcpy(stReturnBookResponse.sErrorDetail, "NO_bookId");
    }

    printf("sUserNum:%s,sbookId:%s\n", stReturnBook.sUserNum, stReturnBook.sbookId);

    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_book where bookId = \"%s\";",
                 stReturnBook.sbookId) <= 0) {
        stReturnBookResponse.iErrorCode = -101;
        strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询数据库是否存在bookId对应的图书
    db_query(*ppvDBHandle, szSqlBuffer, user_return_book_in_callback_ishave, &iCbRet);
    //iCbRet为正数或0，即bookId对应的图书存在 否则返回borrowORreturn_book_response_t错误信息
    if (0 != iCbRet && -100 == stReturnBookResponse.iErrorCode) {
        stReturnBookResponse.iErrorCode = -22;
        strcpy(stReturnBookResponse.sErrorDetail, "LIBRARY_NO_BOOK");
    }
    if (0 == iCbRet && -100 == stReturnBookResponse.iErrorCode) {
        //图书存在 先判断用户是否存在
        //将查询信息打印（复制）到字符串szSqlBuffer
        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum =\"%s\";",
                     stReturnBook.sUserNum) <= 0) {
            stReturnBookResponse.iErrorCode = -101;
            strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
        }
        int iCBAuthentication = -1;
        //查询数据库是否存在usertNum对应的用户
        int flag = db_query(*ppvDBHandle, szSqlBuffer, user_authentication_in_callback, &iCBAuthentication);
        //iCbRet为0，即UserNum对应的用户存在 否则返回borrowORreturn_book_response_t错误信息
        if (0 != flag && -100 == stReturnBookResponse.iErrorCode) {
            stReturnBookResponse.iErrorCode = -24;
            strcpy(stReturnBookResponse.sErrorDetail, "QUERY_FAILURED");
        }
        if (0 == flag && -100 == stReturnBookResponse.iErrorCode) {
            //用户存在 开始还书
            if (0 == iCBAuthentication && -100 == stReturnBookResponse.iErrorCode) {
                //查询用户是否正在借阅这本书
                flag = -1;
                if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                             "select lib_book_borrow.borDate from lib_book_borrow where bookId = \"%s\" and userId= (select lib_user.userId from lib_user where lib_user.userNum = \"%s\") and borStatus=1 limit 0,1;",
                             stReturnBook.sbookId, stReturnBook.sUserNum) <= 0) {
                    stReturnBookResponse.iErrorCode = -101;
                    strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
                }
                //查询数据库是否存在usertNum对应的用户
                flag = db_query(*ppvDBHandle, szSqlBuffer, user_return_book_in_callback_borrow_redate, sRetborDate);
                if (0 != flag && -100 == stReturnBookResponse.iErrorCode) {
                    stReturnBookResponse.iErrorCode = -27;
                    strcpy(stReturnBookResponse.sErrorDetail, "SELECT_FAILURED");
                }
                if (0 == flag && -100 == stReturnBookResponse.iErrorCode) {
                    if (strcmp("",sRetborDate)==0) {
                        stReturnBookResponse.iErrorCode = -26;
                        strcpy(stReturnBookResponse.sErrorDetail, "NO_BORROW_THIS_BOOK");
                    }
                    if (strcmp("",sRetborDate)!=0) {
                        //修改这条借书信息和修改剩余
                        iCbRet = -1;
                        //得到当前归还时间
                        time_t timep;
                        struct tm *p;
                        time(&timep);
                        p = gmtime(&timep);
                        char curRetTime[32];
                        if (snprintf(curRetTime, sizeof(curRetTime), "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
                                     1900 + p->tm_year,
                                     1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec) <= 0) {
                            stReturnBookResponse.iErrorCode = -101;
                            strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
                        }
                        printf("time:%s\n", curRetTime);
                        //将修改信息打印（复制）到字符串szSqlBuffer
                        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                     "update lib_book_borrow, lib_book set lib_book_borrow.retDate = \"%s\",lib_book_borrow.borStatus =0 ,lib_book.bookRemain = lib_book.bookRemain + 1 where lib_book.bookId = \"%s\" and userId= (select lib_user.userId from lib_user where lib_user.userNum = \"%s\") and borDate=\"%s\";",
                                     curRetTime, stReturnBook.sbookId, stReturnBook.sUserNum, sRetborDate) <= 0) {
                            stReturnBookResponse.iErrorCode = -101;
                            strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
                        }
                        //更新借阅信息和剩余信息
                        iCbRet = db_update(*ppvDBHandle, szSqlBuffer);
                        if (0 != iCbRet && -100 == stReturnBookResponse.iErrorCode) {
                            stReturnBookResponse.iErrorCode = -11;
                            strcpy(stReturnBookResponse.sErrorDetail, "UPDATE_FAILURED");
                        }
                        if (0 == iCbRet && -100 == stReturnBookResponse.iErrorCode) {
                            stReturnBookResponse.iErrorCode = 0;
                            strcpy(stReturnBookResponse.sErrorDetail, "LIBRARY_0k");
                        }
                    }
                }
                //查询当前用户是否存在借阅信息
                flag = -1;
                //将查询信息打印（复制）到字符串szSqlBuffer
                if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                             "select count(*) from lib_book_borrow where userId in (select userId from lib_user where userNum = \"%s\");",
                             stReturnBook.sUserNum) <= 0) {
                    stReturnBookResponse.iErrorCode = -101;
                    strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
                }
                //查询数据库是否存在usertNum对应的用户借阅信息
                flag = db_query(*ppvDBHandle, szSqlBuffer, user_return_book_in_callback_total, &iCbRet);
                if (0 != flag && 0 == stReturnBookResponse.iErrorCode) {
                    stReturnBookResponse.iErrorCode = -24;
                    strcpy(stReturnBookResponse.sErrorDetail, "QUERY_FAILURED");
                }
                if (0 == flag) {
                    stReturnBookResponse.itotal = iCbRet;
                    if (stReturnBookResponse.itotal == 0) {
                        stReturnBookResponse.iErrorCode = -25;
                        strcpy(stReturnBookResponse.sErrorDetail, "NO_BORROW_INFO");
                    }
                    if (stReturnBookResponse.itotal > 0) {
                        //查询借书信息
                        //定义一个返回书籍条目信息结构体
                        book_retrun_callback_info_t *sCbRet = (book_retrun_callback_info_t *) malloc(
                                sizeof(book_retrun_callback_info_t) * stReturnBookResponse.itotal);
                        pCbRet = sCbRet;
                        //借书完成 打印当前用户的借阅信息
                        //将查询信息打印（复制）到字符串szSqlBuffer
                        flag = -1;
                        if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                     "select bookName,userNum,bookAuthor,bookCategory,bookPublisher,bookPublicationDate,borRetDateLimit,borDate,retDate from (lib_book_borrow left join lib_book on lib_book_borrow.bookId = lib_book.bookId) left join lib_user on lib_book_borrow.userId = lib_user.userId where  lib_user.userNum =\"%s\";",
                                     stReturnBook.sUserNum) <= 0) {
                            stReturnBookResponse.iErrorCode = -101;
                            strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
                        }
                        //查询数据库是否存在usertNum对应的用户借阅信息
                        flag = db_query(*ppvDBHandle, szSqlBuffer, user_return_book_in_callback_book_item, sCbRet);
                        if (0 != flag && -100 == stReturnBookResponse.iErrorCode) {
                            stReturnBookResponse.iErrorCode = -24;
                            strcpy(stReturnBookResponse.sErrorDetail, "QUERY_FAILURED");
                        }
                    }
                }
            } else {
                stReturnBookResponse.iErrorCode = -3;
                strcpy(stReturnBookResponse.sErrorDetail, "LIBRARY_NO_USER");
            }
        }
    }

/* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON *bookarr = cJSON_CreateArray();
    if (!bookarr) return;
    cJSON_AddNumberToObject(json,
                            "messageId", 4001);
    cJSON_AddNumberToObject(json,
                            "errorCode", stReturnBookResponse.iErrorCode);
    cJSON_AddStringToObject(json,
                            "errorDetail", stReturnBookResponse.sErrorDetail);
    cJSON_AddNumberToObject(json,
                            "total", stReturnBookResponse.itotal);

    for (int i = 0; i < stReturnBookResponse.itotal; i++) {
        cJSON *bookitem = cJSON_CreateObject();
        cJSON_AddStringToObject(bookitem, "bookName", (pCbRet + i)->sbookName);
        cJSON_AddStringToObject(bookitem, "bookNum", (pCbRet + i)->sbookId);
        cJSON_AddStringToObject(bookitem, "bookAuthor", (pCbRet + i)->sbookAuthor);
        cJSON_AddStringToObject(bookitem, "bookCategory", (pCbRet + i)->sbookCategory);
        cJSON_AddStringToObject(bookitem, "bookPublisher", (pCbRet + i)->sbookPublisher);
        cJSON_AddStringToObject(bookitem, "bookPublicationDate", (pCbRet + i)->sbookPublicationDate);
        cJSON_AddStringToObject(bookitem, "borRetDateLimit", (pCbRet + i)->sborRetDateLimit);
        cJSON_AddStringToObject(bookitem, "borDate", (pCbRet + i)->sborData);
        cJSON_AddStringToObject(bookitem, "retDate", (pCbRet + i)->sretDate);
        cJSON_AddItemToArray(bookarr, bookitem);
    }
    cJSON_AddItemToObject(json, "item", bookarr);

    char *pResJson = cJSON_Print(json);
    snprintf(pResponse, iResLen, "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
    free(pCbRet);
}

//4.2用户查询还书信息函数
static void user_search_return_info_book_in(cJSON *root, char *pResponse, int iResLen) {

    borrowORreturn_book_info_t stReturnBook; //创建一个还书信息结构体
    void **ppvDBHandle = get_handle();      //创建一个数据库指针
    char szSqlBuffer[SQL_COMMAND_MAX_SIZE]; //创建一个数据库命令缓冲区
    book_retrun_callback_info_t *pCbRet = NULL;     //定义一个返回书籍条目结构体指针
    int iCbRet = -1;                         //定义一个返回值
    borrowORreturn_book_response_t stReturnBookResponse;       //创建一个返回信息结构体 并赋初值
    stReturnBookResponse.iErrorCode = -100;
    strcpy(stReturnBookResponse.sErrorDetail, "LIBRARY_UNKNOWN_ERROR");
    stReturnBookResponse.itotal = 0;               //初始化返回书籍条目数量

    //判断 json对象中是否含有某一个元素  有：1， 无：0
    //如果有就赋值给还书信息结构体
    if (cJSON_HasObjectItem(root, "userNum")) {
        //将json中userNum打印（复制）到stBorrowBook.sUserNum
        int flag = snprintf(stReturnBook.sUserNum, sizeof(stReturnBook.sUserNum), "%s",
                            cJSON_GetObjectItem(root, "userNum")->valuestring);
        if (flag < 0) {
            stReturnBookResponse.iErrorCode = -101;
            strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
        } else if (flag == 0) {
            stReturnBookResponse.iErrorCode = -13;
            strcpy(stReturnBookResponse.sErrorDetail, "NO_userNum");
        }
    } else {
        stReturnBookResponse.iErrorCode = -13;
        strcpy(stReturnBookResponse.sErrorDetail, "NO_userNum");
    }



    //图书存在 先判断用户是否存在
    //将查询信息打印（复制）到字符串szSqlBuffer
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum =\"%s\";",
                 stReturnBook.sUserNum) <= 0) {
        stReturnBookResponse.iErrorCode = -101;
        strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询数据库是否存在usertNum对应的用户
    int flag = db_query(*ppvDBHandle, szSqlBuffer, user_authentication_in_callback, &iCbRet);
    //iCbRet为0，即UserNum对应的用户存在 否则返回borrowORreturn_book_response_t错误信息
    if (0 != flag && -100 == stReturnBookResponse.iErrorCode) {
        stReturnBookResponse.iErrorCode = -24;
        strcpy(stReturnBookResponse.sErrorDetail, "QUERY_FAILURED");
    }
    if (0 == flag && -100 == stReturnBookResponse.iErrorCode) {
        //用户存在
        if (0 == iCbRet && -100 == stReturnBookResponse.iErrorCode) {
            //查询当前用户是否存在还书信息
            flag = -1;
            //将查询信息打印（复制）到字符串szSqlBuffer
            if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                         "select count(*) from lib_book_borrow where userId in (select userId from lib_user where userNum = \"%s\") and borStatus = 0;",
                         stReturnBook.sUserNum) <= 0) {
                stReturnBookResponse.iErrorCode = -101;
                strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
            }
            //查询数据库是否存在usertNum对应的用户借阅信息
            flag = db_query(*ppvDBHandle, szSqlBuffer, user_return_book_in_callback_total, &iCbRet);
            if (0 != flag && 0 == stReturnBookResponse.iErrorCode) {
                stReturnBookResponse.iErrorCode = -24;
                strcpy(stReturnBookResponse.sErrorDetail, "QUERY_FAILURED");
            }
            if (0 == flag) {
                stReturnBookResponse.itotal = iCbRet;
                if (stReturnBookResponse.itotal == 0) {
                    stReturnBookResponse.iErrorCode = -25;
                    strcpy(stReturnBookResponse.sErrorDetail, "NO_RETURN_INFO");
                }
                if (stReturnBookResponse.itotal > 0) {
                    //查询借书信息
                    //定义一个返回书籍条目信息结构体
                    book_retrun_callback_info_t *sCbRet = (book_retrun_callback_info_t *) malloc(
                            sizeof(book_retrun_callback_info_t) * stReturnBookResponse.itotal);
                    pCbRet = sCbRet;
                    //借书完成 打印当前用户的借阅信息
                    //将查询信息打印（复制）到字符串szSqlBuffer
                    flag = -1;
                    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer),
                                 "select bookName,lib_book.bookId,bookAuthor,bookCategory,bookPublisher,bookPublicationDate,borRetDateLimit,borDate,retDate from (lib_book_borrow left join lib_book on lib_book_borrow.bookId = lib_book.bookId) left join lib_user on lib_book_borrow.userId = lib_user.userId where  lib_user.userNum =\"%s\" and borStatus = 0;",
                                 stReturnBook.sUserNum) <= 0) {
                        stReturnBookResponse.iErrorCode = -101;
                        strcpy(stReturnBookResponse.sErrorDetail, "snprintf_ERROR");
                    }
                    //查询数据库是否存在usertNum对应的用户借阅信息
                    flag = db_query(*ppvDBHandle, szSqlBuffer, user_return_book_in_callback_book_item, sCbRet);
                    if (0 != flag && -100 == stReturnBookResponse.iErrorCode) {
                        stReturnBookResponse.iErrorCode = -24;
                        strcpy(stReturnBookResponse.sErrorDetail, "QUERY_FAILURED");
                    }
                    if (0 == flag && -100 == stReturnBookResponse.iErrorCode) {
                        stReturnBookResponse.iErrorCode = 0;
                        strcpy(stReturnBookResponse.sErrorDetail, "LIBRARY_OK");
                    }
                }
            }
        } else {
            stReturnBookResponse.iErrorCode = -3;
            strcpy(stReturnBookResponse.sErrorDetail, "LIBRARY_NO_USER");
        }
    }


/* 组装数据库为json字符串 */
    cJSON *json = cJSON_CreateObject();
    if (!json) return;
    cJSON *bookarr = cJSON_CreateArray();
    if (!bookarr) return;
    cJSON_AddNumberToObject(json,
                            "messageId", 4002);
    cJSON_AddNumberToObject(json,
                            "errorCode", stReturnBookResponse.iErrorCode);
    cJSON_AddStringToObject(json,
                            "errorDetail", stReturnBookResponse.sErrorDetail);
    cJSON_AddNumberToObject(json,
                            "total", stReturnBookResponse.itotal);

    for (int i = 0; i < stReturnBookResponse.itotal; i++) {
        cJSON *bookitem = cJSON_CreateObject();
        cJSON_AddStringToObject(bookitem,
                                "bookName", (pCbRet + i)->sbookName);
        cJSON_AddStringToObject(bookitem,
                                "bookId", (pCbRet + i)->sbookId);
        cJSON_AddStringToObject(bookitem,
                                "bookAuthor", (pCbRet + i)->sbookAuthor);
        cJSON_AddStringToObject(bookitem,
                                "bookCategory", (pCbRet + i)->sbookCategory);
        cJSON_AddStringToObject(bookitem,
                                "bookPublisher", (pCbRet + i)->sbookPublisher);
        cJSON_AddStringToObject(bookitem,
                                "bookPublicationDate", (pCbRet + i)->sbookPublicationDate);
        cJSON_AddStringToObject(bookitem,"borRetDateLimit", (pCbRet + i)->sborRetDateLimit);
        cJSON_AddStringToObject(bookitem, "borDate", (pCbRet + i)->sborRetDateLimit);
        cJSON_AddStringToObject(bookitem,
                                "retDate", (pCbRet + i)->sretDate);
        cJSON_AddItemToArray(bookarr, bookitem
        );
    }
    cJSON_AddItemToObject(json,
                          "item", bookarr);

    char *pResJson = cJSON_Print(json);
    snprintf(pResponse, iResLen,
             "%s", pResJson);

    cJSON_free(pResJson);
    cJSON_Delete(json);
    free(pCbRet);
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
    if (snprintf(szSqlBuffer, sizeof(szSqlBuffer), "select * from lib_user where userNum =\"%s\";",
                 stUserNum) <= 0) {
        stAdminResponse.iErrorCode = -101;
        strcpy(stAdminResponse.sErrorDetail, "snprintf_ERROR");
    }
    //查询是否有相同的用户
    int flag = db_query(*ppvDBHandle, szSqlBuffer, user_authentication_in_callback, &iCbRet);
    //iCbRet为0，即adminId对应的管理员存在 否则返回admin_response_t错误信息
    if (0 != flag && -100 == stAdminResponse.iErrorCode) {
        stAdminResponse.iErrorCode = -24;
        strcpy(stAdminResponse.sErrorDetail, "QUERY_FAILURED");
    }
    if (0 == flag && -100 == stAdminResponse.iErrorCode) {
        if (0 == iCbRet) {
            stAdminResponse.iErrorCode = 0;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_0k");
        } else {
            stAdminResponse.iErrorCode = -3;
            strcpy(stAdminResponse.sErrorDetail, "LIBRARY_NO_USER");
        }
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
        case 2001:
            display_book_in(root, pResponse, iResLen);
            break;
        case 2002:
            book_storage_in(root, pResponse, iResLen);
            break;
        case 2003:
            search_book_in(root, pResponse, iResLen);
            break;
        case 3001:
            user_borrow_book_in(root, pResponse, iResLen);
            break;
        case 3002:
            user_search_borrow_info_book_in(root, pResponse, iResLen);
            break;
        case 4001:
            user_return_book_in(root, pResponse, iResLen);
            break;
        case 4002:
            user_search_return_info_book_in(root, pResponse, iResLen);
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

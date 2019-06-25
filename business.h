//
// Created by yang on 2019/6/24.
//
#ifndef _BUSINESS_H_
#define _BUSINESS_H_

typedef struct admin_info {
    char sAdminId[24];
    char sAdminPasswd[24];
    char sAdminName[24];
}admin_info_t;

typedef struct admin_response {
    int iErrorCode;
    char sErrorDetail[24];
}admin_response_t;

typedef struct book_info{
    char sbookId[24];
    char sbookName[50];
    char sbookAuthor[24];
    int ibookStock;
    int ibookRemain;
    int ibookTimes;
    char sbookCategory[24];
    char sbookPublisher[50];
    char sbookPublicationDate[24];
}book_item_info_t;


void** get_handle();
int exec_business(const char *pRequest, int iReqLen, char *pResponse, int iResLen);

#endif


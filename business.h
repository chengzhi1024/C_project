#ifndef _BUSINESS_H_
#define _BUSINESS_H_

//管理员信息结构体
typedef struct admin_info {
    char sAdminId[24];
    char sAdminPasswd[24];
    char sAdminName[24];
}admin_info_t;

//管理员修改密码结构体
typedef struct admin_modifypassword{
    char sAdminId[24];
    char sAdminOldPasswd[24];
    char sAdminNewPasswd[24];
}admin_modifypassword_t;

//登录返回信息结构体
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

//用户信息结构体
typedef struct user_info{
    char sUserNum[24];
    char sUserName[24];
    char sUserSex[4];
    char sUserRemark[50];
}user_info_t;

void** get_handle();
int exec_business(const char *pRequest, int iReqLen, char *pResponse, int iResLen);

#endif

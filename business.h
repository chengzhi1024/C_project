#ifndef _BUSINESS_H_
#define _BUSINESS_H_


//第一部分 管理员结构体部分
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


//第二部分 图书结构体部分
//图书信息结构体
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
}book_info_t;
//图书信息显示位置结构体
typedef struct book_display_position{
    int iStart;
    int iEnd;
}book_display_position_t;
//图书搜索信息结构体
typedef struct book_search_info{
    char sBookSearch[24];
    int iType;   // 0代表书号，1代表书名，2代表作者
}book_search_info_t;
//图书显示返回错误信息结构体
typedef struct book_display_response_info{
    int iErrorCode;
    char sErrorDetail[24];
    int itotal;
//    book_info_t* item;
}book_display_response_t;


//第三部分 借书还书结构体部分
//借书还书信息结构体
typedef struct borrowORreturn_book_info{
    char sUserNum[24];
    char sbookId[24];
}borrowORreturn_book_info_t;
//借书还书返回错误信息结构体
typedef struct borrowORreturn_response_info{
    int iErrorCode;
    char sErrorDetail[24];
    int itotal;
//    book_info_t* item;
}borrowORreturn_book_response_t;
//借书返回信息结构体
typedef struct book_borrow_callback_info{
    char sbookName[50];
    char sbookId[24];
    char sbookAuthor[24];
    char sbookCategory[24];
    char sbookPublisher[50];
    char sbookPublicationDate[24];
    char sborRetDateLimit[24];
    char sborData[24];
}book_borrow_callback_info_t;
//还书返回信息结构体
typedef struct book_retrun_callback_info{
    char sbookName[50];
    char sbookId[24];
    char sbookAuthor[24];
    char sbookCategory[24];
    char sbookPublisher[50];
    char sbookPublicationDate[24];
    char sborRetDateLimit[24];
    char sborData[24];
    char sretDate[24];
}book_retrun_callback_info_t;


//第四部分 用户结构体部分
//用户信息结构体
typedef struct user_info{
    char sUserNum[24];
    char sUserName[24];
    char sUserSex[4];
    char sUserRemark[50];
}user_info_t;


//第五部分 公共结构体部分
//错误信息返回结构体
typedef struct admin_response {
    int iErrorCode;
    char sErrorDetail[24];
}admin_response_t;


void** get_handle();
int exec_business(const char *pRequest, int iReqLen, char *pResponse, int iResLen);

#endif

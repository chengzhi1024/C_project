#ifndef _BUSINESS_H_
#define _BUSINESS_H_

//管理员信息结构体
typedef struct admin_info {
    char sAdminId[48];
    char sAdminPasswd[48];
    char sAdminName[48];
}admin_info_t;

//登录返回信息结构体
typedef struct admin_response {
	int iErrorCode;
	char sErrorDetail[48];
}admin_response_t;

void** get_handle();
int exec_business(const char *pRequest, int iReqLen, char *pResponse, int iResLen);

#endif

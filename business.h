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

void** get_handle();
int exec_business(const char *pRequest, int iReqLen, char *pResponse, int iResLen);

#endif

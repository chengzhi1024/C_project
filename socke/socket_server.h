#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H

typedef int (*recv_handle_t)(const char *pRequest, int iReqLen, char *pResponse, int iResLen);

int socket_init(short nPort);
int socket_accept(int iSockFd, recv_handle_t cb);
void socket_uninit(int iSockFd);

#endif

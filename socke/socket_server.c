#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <cJSON.h>
#include <db_interface.h>
#include <socket_server.h>

int socket_init(short nPort)
{
	// 设置一个socket地址结构server_addr,代表服务器internet的地址和端口
    struct sockaddr_in   server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(nPort);

    // 创建用于internet的流协议(TCP)socket，用server_socket代表服务器向客户端提供服务的接口
    int iSocketFd = socket(PF_INET, SOCK_STREAM, 0);
    if (iSocketFd < 0) {
        printf("Create Socket Failed!\n");
        return -1;
    }
	
	// 复用端口号
	int opt = SO_REUSEADDR;
    setsockopt(iSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 把socket和socket地址结构绑定
    if (bind(iSocketFd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        printf("Server Bind Port: %d Failed!\n", nPort);
        close(iSocketFd);
		return -1;
    }

    // server_socket用于监听
    if (listen(iSocketFd, 20)) {
        printf("Server Listen Failed!\n");
        close(iSocketFd);
		return -1;
    }
	return iSocketFd;
}

int socket_accept(int iSockFd, recv_handle_t callback)
{
	int iConnFd;
	socklen_t stAddrLen;
	struct sockaddr_in client;
	char sRecvbuf[1024];
	char sSendBuf[1024];

	stAddrLen = sizeof(client);
	while(1) {
		if((iConnFd = accept(iSockFd,(struct sockaddr *)&client, &stAddrLen)) == -1) {
			perror("accept() error. \n");
			continue;
		}

		struct timeval tv;
		gettimeofday(&tv, NULL);
		printf("You got a connection from client's ip %s, port %d at time %ld.%ld\n",
			inet_ntoa(client.sin_addr), htons(client.sin_port), tv.tv_sec,tv.tv_usec);

		int iRet = -1;
		while(1)
		{
			iRet = recv(iConnFd, sRecvbuf, sizeof(sRecvbuf), 0);
			if (iRet >0 ) {
				printf("recv:%s\n", sRecvbuf);
				callback(sRecvbuf, sizeof(sRecvbuf), sSendBuf, sizeof(sSendBuf));
			}
			else {
				close(iConnFd);
				break;
			}
			printf("send:%s\n", sSendBuf);
			send(iConnFd, sSendBuf, strlen(sSendBuf), 0);
		}
	}
}

void socket_uninit(int iSockFd)
{
	close(iSockFd);
}


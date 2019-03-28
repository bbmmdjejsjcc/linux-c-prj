#include <sys/un.h>
#include <sys/socket.h>
#include "local_client.h"
#include <stdio.h>

struct sockaddr_un socketAddr;
int sfd;//该socket当服务器
int cfd;
ssize_t sNumRead;
ssize_t sNumWrite;
char buf[BUF_SIZE];
int localSockInit(void)
{
	/*创建socket*/
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	if(sfd==-1)
		printf("socket error\n");

	/*移除原有的socket路径地址*/
	if(remove(LOCAL_SOCK_PATH)==-1)
		printf("remove error\n");

	/*初始化addr*/
	memset(&socketAddr,0,sizeof(struct sockaddr_un));
	socketAddr.sun_family=AF_UNIX;
	strncpy(socketAddr.sun_path,LOCAL_SOCK_PATH,sizeof(socketAddr.sun_path)-1);

	/*将socket绑定到addr*/
	if(bind(sfd,(struct sockaddr *)&socketAddr,sizeof(struct sockaddr_un))==-1)
		printf("bind error\n");

	/*liste,表示愿意接收接入*/
	if(listen(sfd,BACKLOG)==-1)
		printf("listen error\n");

	return 0;
}







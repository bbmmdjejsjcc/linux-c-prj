#include <sys/un.h>
#include <sys/socket.h>
#include "local_client.h"
#include <stdio.h>

struct sockaddr_un socketAddr;
int sfd;//��socket��������
int cfd;
ssize_t sNumRead;
ssize_t sNumWrite;
char buf[BUF_SIZE];
int localSockInit(void)
{
	/*����socket*/
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	if(sfd==-1)
		printf("socket error\n");

	/*�Ƴ�ԭ�е�socket·����ַ*/
	if(remove(LOCAL_SOCK_PATH)==-1)
		printf("remove error\n");

	/*��ʼ��addr*/
	memset(&socketAddr,0,sizeof(struct sockaddr_un));
	socketAddr.sun_family=AF_UNIX;
	strncpy(socketAddr.sun_path,LOCAL_SOCK_PATH,sizeof(socketAddr.sun_path)-1);

	/*��socket�󶨵�addr*/
	if(bind(sfd,(struct sockaddr *)&socketAddr,sizeof(struct sockaddr_un))==-1)
		printf("bind error\n");

	/*liste,��ʾԸ����ս���*/
	if(listen(sfd,BACKLOG)==-1)
		printf("listen error\n");

	return 0;
}







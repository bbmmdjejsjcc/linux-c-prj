#include <sys/un.h>
#include <sys/socket.h>
#include "local_client.h"
#include <stdio.h>

#define LOCAL_SOCK_PATH "/local_sock"
#define BACKLOG 5

char bufRx[4096];
char bufS[4096];
char *pSockR;
int sNumRead;


//socket线程函数
void *socketThread(void *arg)
{
		int i=0;
		//struct sockaddr_un client_addr;
		struct sockaddr_un server_addr;
		int sfd,cfd;
		ssize_t numRead;
		char bufS[4096];
		char bufRx[4096];
		int sentCnt;
		int value='1';

		struct timespec ts,ts1;
//		ts.tv_nsec=50000000;
		ts.tv_nsec=50000000;
		ts.tv_sec=0;
		
		/*打印出主线程传递进来的参数*/
		char *s=(char *)arg;
		printf("%s",s);
		
		pSockR=bufRx;
		/*创建socket*/
			sfd=socket(AF_LOCAL,SOCK_STREAM,0);
			if(sfd==-1)
				printf("socket error\n");

		
		/*移除原有的socket路径地址*/
		if(remove(LOCAL_SOCK_PATH)==-1)
			printf("remove error\n");

		/*初始化addr*/
		memset(&server_addr,0,sizeof(struct sockaddr_un));
		server_addr.sun_family=AF_LOCAL;
		strncpy(server_addr.sun_path,LOCAL_SOCK_PATH,sizeof(server_addr.sun_path)-1);

		/*将socket绑定到addr*/
		if(bind(sfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_un))==-1)
			printf("bind error\n");

		/*liste,表示愿意接收接入*/
		if(listen(sfd,BACKLOG)==-1)
			printf("listen error\n");

		printf("i am waitting to be connected\n");
		//accept调用实际上是新建了一个socket，返回到cfd，来和client进行通信
		cfd=accept(sfd,NULL,NULL);//阻塞在这里，直至客户端来连接
		printf("i had been connnected\n");
		if(cfd==-1)
			printf("accept error\n");

		while(1)
			{
				for(i=0;i<20;i++)
					{
						bufS[0]=i*5;
					
				if(write(cfd,bufS,20)!=20)
					printf("server write error\n");
					

				printf("i am server\n");
				nanosleep(&ts, &ts1);//50ms
					}
			}

	}






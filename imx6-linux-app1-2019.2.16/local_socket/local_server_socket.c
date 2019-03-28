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


//socket�̺߳���
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
		
		/*��ӡ�����̴߳��ݽ����Ĳ���*/
		char *s=(char *)arg;
		printf("%s",s);
		
		pSockR=bufRx;
		/*����socket*/
			sfd=socket(AF_LOCAL,SOCK_STREAM,0);
			if(sfd==-1)
				printf("socket error\n");

		
		/*�Ƴ�ԭ�е�socket·����ַ*/
		if(remove(LOCAL_SOCK_PATH)==-1)
			printf("remove error\n");

		/*��ʼ��addr*/
		memset(&server_addr,0,sizeof(struct sockaddr_un));
		server_addr.sun_family=AF_LOCAL;
		strncpy(server_addr.sun_path,LOCAL_SOCK_PATH,sizeof(server_addr.sun_path)-1);

		/*��socket�󶨵�addr*/
		if(bind(sfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_un))==-1)
			printf("bind error\n");

		/*liste,��ʾԸ����ս���*/
		if(listen(sfd,BACKLOG)==-1)
			printf("listen error\n");

		printf("i am waitting to be connected\n");
		//accept����ʵ�������½���һ��socket�����ص�cfd������client����ͨ��
		cfd=accept(sfd,NULL,NULL);//���������ֱ���ͻ���������
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






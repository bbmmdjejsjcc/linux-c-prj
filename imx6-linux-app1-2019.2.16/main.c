#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>


#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


#include "update.h"
#include "local_server_socket.h"


//spi�̺߳���
static void *spiDatRecThread(void *arg)
{
	/*��ӡ�����̴߳��ݽ����Ĳ���*/
	char *s=(char *)arg;
	printf("%s",s);
	while(1)
		{
			;
		}
}

//��Ƶ�����̺߳���
static void *playWavThread(void *arg)
{
	/*��ӡ�����̴߳��ݽ����Ĳ���*/
	char *s=(char *)arg;
	printf("%s",s);
	//������Ƶ����
//	setVol(110);
//	printf("finish vol set\n");
//	printf("first time play\n");
//	printf("second time play\n");
//	playwav("time.wav");
	while(1)
		{
			;
		}
}


void spiRecDatParse(void)
{
	;
}


int main()
{
	pthread_t t1,t2,t3,t4,t5,t6;
	int s;
	int cnt=0;

	/*�����̣߳����ڽ��������ӣ��Լ��շ�socket����*/
//	s=pthread_create(&t1,NULL,socketThread,"Hello socketThread\n");
//	if(s!=0)
//		printf("socketThread create error\n");

	/*�����̣߳����ڽ���spi���ݼ�У������*/
	s=pthread_create(&t2,NULL,spiDatRecThread,"Hello spiDatAnalysisThread\n");
	if(s!=0)
		printf("spiDatAnalysisThread create error\n");

	/*�����̣߳����ڲ�����Ƶ*/
	s=pthread_create(&t3,NULL,playWavThread,"Hello playWavThread\n");
	if(s!=0)
		printf("playWavThread create error\n");

	/*�����̣߳����ڼ�ⰴ��*/
//	s=pthread_create(&t4,NULL,keyThread,"Hello keyThread\n");
//	if(s!=0)
//		printf("keyThread create error\n");

	/*�����̣߳����ڼ��sd���Ƿ���ڣ�������usb���������*/
	s=pthread_create(&t5,NULL,usbUpdateThread,"Hello usbUpdateThread\n");
	if(s!=0)
		printf("usbUpdateThread create error\n");

//	/*�����̣߳����ڶ�ȡrtcʱ��*/
//		s=pthread_create(&t6,NULL,rtcThread,"Hello rtcThread\n");
//		if(s!=0)
//			printf("rtcThread create error\n");


	//���߳��������������ݣ���������������ͷ��ʾ��
	while(1)
		{
			//spi�յ������ݽ���
			spiRecDatParse();

			
			//����ͷ�����رղ���
//			for(cnt=0;cnt<10;cnt++)
//				{
//					oepnOv5642(1024,600);
//					sleep(5);
//					colsoOv5642();
//					sleep(10);
//				}
			
		}
	
	return 0;
}

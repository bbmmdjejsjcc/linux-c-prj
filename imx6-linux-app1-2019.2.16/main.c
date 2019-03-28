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


//spi线程函数
static void *spiDatRecThread(void *arg)
{
	/*打印出主线程传递进来的参数*/
	char *s=(char *)arg;
	printf("%s",s);
	while(1)
		{
			;
		}
}

//音频播放线程函数
static void *playWavThread(void *arg)
{
	/*打印出主线程传递进来的参数*/
	char *s=(char *)arg;
	printf("%s",s);
	//播放音频测试
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

	/*创建线程，用于建立长连接，以及收发socket数据*/
//	s=pthread_create(&t1,NULL,socketThread,"Hello socketThread\n");
//	if(s!=0)
//		printf("socketThread create error\n");

	/*创建线程，用于接收spi数据及校验数据*/
	s=pthread_create(&t2,NULL,spiDatRecThread,"Hello spiDatAnalysisThread\n");
	if(s!=0)
		printf("spiDatAnalysisThread create error\n");

	/*创建线程，用于测试音频*/
	s=pthread_create(&t3,NULL,playWavThread,"Hello playWavThread\n");
	if(s!=0)
		printf("playWavThread create error\n");

	/*创建线程，用于检测按键*/
//	s=pthread_create(&t4,NULL,keyThread,"Hello keyThread\n");
//	if(s!=0)
//		printf("keyThread create error\n");

	/*创建线程，用于检测sd卡是否存在，及进行usb程序的升级*/
	s=pthread_create(&t5,NULL,usbUpdateThread,"Hello usbUpdateThread\n");
	if(s!=0)
		printf("usbUpdateThread create error\n");

//	/*创建线程，用于读取rtc时间*/
//		s=pthread_create(&t6,NULL,rtcThread,"Hello rtcThread\n");
//		if(s!=0)
//			printf("rtcThread create error\n");


	//主线程来解析部分数据，语音播报，摄像头显示等
	while(1)
		{
			//spi收到的数据解析
			spiRecDatParse();

			
			//摄像头开启关闭测试
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

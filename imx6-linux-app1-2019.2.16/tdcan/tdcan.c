#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <string.h>
#include <stdio.h>
#include <linux/can/raw.h>
#include "tdcan.h"

int can_s;
unsigned long nbytes;
struct sockaddr_can addr;
struct ifreq ifr;
struct can_frame sendFrame;
struct can_frame rxFrame;
//struct can_filter rfilter;

void tdCanInit(void)
{
	int loopback=0;//0��ʾ�رգ�1��ʾ����(Ĭ��)
	/*�����׽���*/
	can_s = socket(PF_CAN,SOCK_RAW,CAN_RAW);

	/*ָ��can0�豸*/
	strcpy((char *)(ifr.ifr_name),"can0");
	ioctl(can_s,SIOCGIFINDEX,&ifr);
	printf("can0 can_ifindex = %x\n",ifr.ifr_ifindex);

	//rfilter.can_mask=CAN_SFF_MASK;
	//setsockopt(can_s,SOL_CAN_RAW,CAN_RAW_FILTER,&rfilter,sizeof(rfilter));

	/*���׽�����can0��*/
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(can_s,(struct sockaddr*)&addr,sizeof(addr));

	/*Ҫ���͵ı���*/
	sendFrame.can_id = 0x12345678;
	strcpy((char *)sendFrame.data,"hello");
	sendFrame.can_dlc = strlen(sendFrame.data);
	printf("Send a CAN frame from interface %s\n",ifr.ifr_name);

	/*��ֹ����ģʽ*/
	setsockopt(can_s,SOL_CAN_RAW,CAN_RAW_LOOPBACK,&loopback,sizeof(loopback));
}




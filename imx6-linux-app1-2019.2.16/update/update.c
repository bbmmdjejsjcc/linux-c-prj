#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "update.h"

void *usbUpdateThread(void *arg)
{
	int uFd=0;
	/*��ӡ�����̴߳��ݽ����Ĳ���*/
	char *s=(char *)arg;
	printf("%s",s);
	
	printf("ProgrameVersion=%d\n",ProgrameVersion);
	uFd=open("/dev/sda1",O_RDWR);//��u��,�������ķ�ʽ��
	if(uFd<0)
		{
			printf("no storage dev found\n");
		}
	else
		{
			printf("open u Storage ok,uFd=%d\n",uFd);
		}
	system("echo hello");
	system("mount -t vfat /dev/sda1 /mount_u_storage");
	system("cp /mount_u_storage/update-new /");
	system("/update-new");
//	system("cat /file-test.txt");
	close(uFd);
}



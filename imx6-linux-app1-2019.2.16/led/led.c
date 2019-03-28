#include   <stdio.h>  
#include   <stdlib.h>
#include   <string.h>  
#include   <unistd.h>  
#include   <fcntl.h>  
#include   <errno.h>  
#include   <termios.h>  
#include   <sys/time.h>
#include <linux/input.h>

void write_int(char *path, int i)
{
	char buf[1024];
	int fd = open(path, O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}
	sprintf(buf, "%d", i);
	if(write(fd, buf, strlen(buf)) != strlen(buf)) {
		perror("write");
		exit(1);
	}
	close(fd);
}

void set_led(int on)
{
	write_int("/sys/class/leds/user_led/brightness", on);
//	write_int("/sys/class/leds/led1/brightness", on);
}

int main(int argc,char **argv)
{
	int v=1;
	int i;
	for(i=0; i<100000; i++)
	{
		if(v==0)
			v=1;
		else
			v=0;	
	printf("write=%d\n",v);
	set_led(v);
	usleep(1000000);
	}
}

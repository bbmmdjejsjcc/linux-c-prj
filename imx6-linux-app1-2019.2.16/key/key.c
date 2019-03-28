#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#define NOKEY 0
#define KEYPAD_DEV /dev/input/event0

int main()
{
	int keys_fd;	
    char ret[2]; 
	struct input_event t;
	char *dev;
      
    keys_fd = open("/dev/input/event0", O_RDWR);
	if(keys_fd<=0)
		{
	        printf("open %s device error!\n",keys_fd);
			return 0;
		}
//	printf("size of t is %d\n",sizeof(t));

	while(1)
	{	
        if(read(keys_fd,&t,sizeof(t))==sizeof(t)) //block read
		{
			printf("read finish\n");
			printf("t.type=%d\n",t.type);
			printf("t.code=%d\n",t.code);
			printf("t.value=%d\n",t.value);
		    if(t.type==EV_KEY)
				if(t.value==0 || t.value==1)
					{
//						printf("%d\n", t.code);
						switch(t.code)
						{
							case 103:
					    		printf("key103 %s\n",(t.value)?"Pressed":"Released");
					    		break;
							
							case 105:
					    		printf("key105 %s\n",(t.value)?"Pressed":"Released");
					    		break;
					    	
					    	case 106:
					    		printf("key106 %s\n",(t.value)?"Pressed":"Released");
					    		break;

							case 108:
					    		printf("key108 %s\n",(t.value)?"Pressed":"Released");
					    		break;
					    			    	   	
					    	default:
					    		break;
					    }
					}
				printf("\n");
		}
	}	
	close(keys_fd);
    return 0;
}


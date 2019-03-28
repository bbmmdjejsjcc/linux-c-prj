#ifndef _TDCAN_H
#define _TDCAN_H

#include <net/if.h>
#include <linux/can.h>


#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

extern int can_s;
extern unsigned long nbytes;
extern struct sockaddr_can addr;
extern struct ifreq ifr;
extern struct can_frame sendFrame;
extern struct can_frame rxFrame;
//extern struct can_filter rfilter;


extern void tdCanInit(void);


#endif





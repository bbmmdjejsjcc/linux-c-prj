/*
 * Copyright 2004-2013 Freescale Semiconductor, Inc. All rights reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*
 * @file mxc_v4l2_overlay.c
 *
 * @brief Mxc Video For Linux 2 driver test application
 *
 */

#ifdef __cplusplus
extern "C"{
#endif

/*=======================================================================
                                        INCLUDE FILES
=======================================================================*/
/* Standard Include Files */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* Verification Test Environment Include Files */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>

#include "mxcfb.h"

#define TFAIL -1
#define TPASS 0

#define ipu_fourcc(a,b,c,d)\
        (((__u32)(a)<<0)|((__u32)(b)<<8)|((__u32)(c)<<16)|((__u32)(d)<<24))

#define IPU_PIX_FMT_RGB332  ipu_fourcc('R','G','B','1') /*!<  8  RGB-3-3-2     */
#define IPU_PIX_FMT_RGB555  ipu_fourcc('R','G','B','O') /*!< 16  RGB-5-5-5     */
#define IPU_PIX_FMT_RGB565  ipu_fourcc('R','G','B','P') /*!< 16  RGB-5-6-5     */
#define IPU_PIX_FMT_RGB666  ipu_fourcc('R','G','B','6') /*!< 18  RGB-6-6-6     */
#define IPU_PIX_FMT_BGR24   ipu_fourcc('B','G','R','3') /*!< 24  BGR-8-8-8     */
#define IPU_PIX_FMT_RGB24   ipu_fourcc('R','G','B','3') /*!< 24  RGB-8-8-8     */
#define IPU_PIX_FMT_BGR32   ipu_fourcc('B','G','R','4') /*!< 32  BGR-8-8-8-8   */
#define IPU_PIX_FMT_BGRA32  ipu_fourcc('B','G','R','A') /*!< 32  BGR-8-8-8-8   */
#define IPU_PIX_FMT_RGB32   ipu_fourcc('R','G','B','4') /*!< 32  RGB-8-8-8-8   */
#define IPU_PIX_FMT_RGBA32  ipu_fourcc('R','G','B','A') /*!< 32  RGB-8-8-8-8   */
#define IPU_PIX_FMT_ABGR32  ipu_fourcc('A','B','G','R') /*!< 32  ABGR-8-8-8-8  */

char v4l_device[100] = "/dev/video0";
int fd_v4l = 0;
int g_sensor_width = 1920;
int g_sensor_height = 1080;
//int g_sensor_width = 1440;
//int g_sensor_height = 540;

int g_sensor_top = 0;
int g_sensor_left = 0;
//int g_display_width = 240;
//int g_display_height = 320;
//int g_display_top = 0;
//int g_display_left = 0;

//int g_display_width = 640;
//int g_display_height = 480;
int g_display_width = 300;
int g_display_height = 200;
int g_display_top = 0;
int g_display_left = 0;

int g_rotate = 4;
int g_timeout = 3600;
int g_display_lcd = 0;
int g_overlay = 0;
int g_camera_color = 0;
int g_camera_framerate = 30;
int g_capture_mode = 0;
int g_alpha_mode = 0;
char *alpha_buf0 = NULL;
char *alpha_buf1 = NULL;
int alpha_fb_w = 0, alpha_fb_h = 0;
unsigned long loc_alpha_phy_addr0;
unsigned long loc_alpha_phy_addr1;
int alpha_buf_size = 0;
int ctrl_c_rev = 0;
int g_fd_fb_fg = 0;

static void print_pixelformat(char *prefix, int val)
{
	printf("%s: %c%c%c%c\n", prefix ? prefix : "pixelformat",
					val & 0xff,
					(val >> 8) & 0xff,
					(val >> 16) & 0xff,
					(val >> 24) & 0xff);
}

void ctrl_c_handler(int signum, siginfo_t *info, void *myact)
{
	ctrl_c_rev = 1;
	return;
}

/* fill in alpha value to a part of alpha buffer */
void fill_alpha_buffer(char *alpha_buf, int left, int top,
		       int right, int bottom, char alpha_val)
{
	char *pPointAlphaValue;
	int x, y;

	for (y = top; y < bottom; y++) {
		for (x = left; x < right; x++) {
			pPointAlphaValue = (char *)(alpha_buf +
					    alpha_fb_w * y + x);
			*pPointAlphaValue = alpha_val;
		}
	}
}

int
mxc_v4l_overlay_test(int timeout)
{
        int i;
        int overlay = 1;
        int retval = 0;
        struct v4l2_control ctl;
//#ifdef BUILD_FOR_ANDROID
//	char fb_device_1[100] = "/dev/graphics/fb0";
//#else
	
	char fb_device_1[100] = "/dev/fb1";
//#endif
	int fd_graphic_fb = 0;
	struct fb_var_screeninfo fb0_var;
	struct mxcfb_loc_alpha l_alpha;

	if (ioctl(fd_v4l, VIDIOC_OVERLAY, &overlay) < 0) {
		printf("VIDIOC_OVERLAY start failed\n");
		retval = TFAIL;
		goto out1;
	}

	printf("g_alpha_mode=%d\n",g_alpha_mode);



        for (i = 0; i < 3 ; i++) {
                // flash a frame
                ctl.id = V4L2_CID_PRIVATE_BASE + 1;
                if (ioctl(fd_v4l, VIDIOC_S_CTRL, &ctl) < 0)
                {
                        printf("set ctl failed\n");
			retval = TFAIL;
			goto out2;
                }
	            sleep(1);
        }

		printf("location b\n");
		printf("g_camera_color=%d\n",g_camera_color);




		sleep(timeout);

out2:
        overlay = 0;
        if (ioctl(fd_v4l, VIDIOC_OVERLAY, &overlay) < 0)
        {
                printf("VIDIOC_OVERLAY stop failed\n");
		retval = TFAIL;
		goto out1;
        }
out1:
	if (g_alpha_mode) {
		munmap((void *)alpha_buf0, alpha_buf_size);
		munmap((void *)alpha_buf1, alpha_buf_size);

		/*
		 * Disable DP local alpha function, otherwise,
		 * the alpha channel will be enabled even if we
		 * use DP global alpha next time and this will
		 * cause display issue.
		 */
		l_alpha.enable = 0;
		l_alpha.alpha_phy_addr0 = 0;
		l_alpha.alpha_phy_addr1 = 0;
		if (ioctl(fd_graphic_fb, MXCFB_SET_LOC_ALPHA, &l_alpha) < 0) {
			printf("Set local alpha failed\n");
			retval = TFAIL;
			goto out0;
		}
out0:
		if (!g_overlay) {
			if (ioctl(g_fd_fb_fg, FBIOBLANK, FB_BLANK_POWERDOWN) < 0) {
				printf("Unblank overlay frame buffer failed\n");
				close(g_fd_fb_fg);
				return TFAIL;
			}
			close(g_fd_fb_fg);
		}
	}

	close(fd_graphic_fb);
	return retval;
}

int
mxc_v4l_overlay_setup(struct v4l2_format *fmt)
{
        struct v4l2_streamparm parm;
        v4l2_std_id id;
        struct v4l2_control ctl;
        struct v4l2_crop crop;
	struct v4l2_frmsizeenum fsize;
	struct v4l2_fmtdesc ffmt;

	printf("sensor supported frame size:\n");
	fsize.index = 0;
	while (ioctl(fd_v4l, VIDIOC_ENUM_FRAMESIZES, &fsize) >= 0) {
		printf(" %dx%d\n", fsize.discrete.width,
					       fsize.discrete.height);
		fsize.index++;
	}

	ffmt.index = 0;
	while (ioctl(fd_v4l, VIDIOC_ENUM_FMT, &ffmt) >= 0) {
		print_pixelformat("sensor frame format", ffmt.pixelformat);
		ffmt.index++;
	}

	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	parm.parm.capture.timeperframe.numerator = 1;
	parm.parm.capture.timeperframe.denominator = g_camera_framerate;
	parm.parm.capture.capturemode = g_capture_mode;

//	if (ioctl(fd_v4l, VIDIOC_S_PARM, &parm) < 0)
//	{
//	        printf("VIDIOC_S_PARM failed\n");
//	        return TFAIL;
//	}

	parm.parm.capture.timeperframe.numerator = 0;
	parm.parm.capture.timeperframe.denominator = 0;

	if (ioctl(fd_v4l, VIDIOC_G_PARM, &parm) < 0)
	{
	        printf("get frame rate failed\n");
	        return TFAIL;
	}

	printf("frame_rate is %d\n",
	       parm.parm.capture.timeperframe.denominator);

	ctl.id = V4L2_CID_PRIVATE_BASE + 2;
		ctl.value = g_rotate;
        if (ioctl(fd_v4l, VIDIOC_S_CTRL, &ctl) < 0)
        {
                printf("set control failed\n");
                return TFAIL;
        }

        crop.type = V4L2_BUF_TYPE_VIDEO_OVERLAY;
        crop.c.left = g_sensor_left;
        crop.c.top = g_sensor_top;
        crop.c.width = g_sensor_width;
        crop.c.height = g_sensor_height;
        if (ioctl(fd_v4l, VIDIOC_S_CROP, &crop) < 0)
        {
                printf("set cropping failed\n");
                return TFAIL;
        }

        if (ioctl(fd_v4l, VIDIOC_S_FMT, fmt) < 0)
        {
                printf("set format failed\n");
                return TFAIL;
        }

        if (ioctl(fd_v4l, VIDIOC_G_FMT, fmt) < 0)
        {
                printf("get format failed\n");
                return TFAIL;
        }

        if (ioctl(fd_v4l, VIDIOC_G_STD, &id) < 0)
        {
                printf("VIDIOC_G_STD failed\n");
                return TFAIL;
        }

        return TPASS;
}


int oepnOv5642(int width, int height)
{
		int retval = 0;
        struct v4l2_format fmt;
        struct v4l2_framebuffer fb_v4l2;
//#ifdef BUILD_FOR_ANDROID
//        char fb_device_1[100] = "/dev/graphics/fb1";
//#else
		//new1
		char fb_device_0[100] = "/dev/fb0";//其他用，背景层
        char fb_device_1[100] = "/dev/fb1";//摄像头用，前景层
//#endif
        char *fb_device_fg;

		//new2
		int fd_fb_0 = 0;
        int fd_fb_1 = 0;
		int g_fb1_size=0;
		unsigned short * fb1;
		
		int testoverlay=1;
        struct fb_fix_screeninfo fb0_fix, fb_fg_fix;
        struct fb_var_screeninfo fb0_var, fb_fg_var;

        struct mxcfb_color_key color_key;
        struct mxcfb_gbl_alpha g_alpha;
        struct mxcfb_loc_alpha l_alpha;
	int ret = 0;
//	struct v4l2_dbg_chip_ident chip;

        if ((fd_v4l = open(v4l_device, O_RDWR, 0)) < 0) {
                printf("Unable to open %s\n", v4l_device);
                return TFAIL;
        }

//	if (ioctl(fd_v4l, VIDIOC_DBG_G_CHIP_IDENT, &chip))//打开摄像头
//	{
//                printf("VIDIOC_DBG_G_CHIP_IDENT failed.\n");
//                return TFAIL;
//	}
//	printf("sensor chip is %s\n", chip.match.name);

	//new3-start
//	if ((fd_fb_0 = open(fb_device_0, O_RDWR )) < 0) //打开fb0	背景层
//		{
//		printf("Unable to open frame buffer 0\n");
//                return TFAIL;
//        }
	//new3-end

	if ((fd_fb_1 = open(fb_device_1, O_RDWR )) < 0) //打开fb1	前景层
		{
		printf("Unable to open frame buffer 1\n");
                return TFAIL;
        }

	if (ioctl(fd_fb_1, FBIOGET_FSCREENINFO, &fb_fg_fix) < 0)//获取fb1固定信息 
		{
            close(fd_fb_1);
            return TFAIL;
    	}

	if (ioctl(fd_fb_1, FBIOGET_VSCREENINFO, &fb_fg_var) < 0)//获取fb1可变信息 
			{
                close(fd_fb_1);
                return TFAIL;
        	}

	//new4-start
		fb_fg_var.bits_per_pixel = 16;
        fb_fg_var.yoffset = 0;
//		fb_fg_var.yres_virtual=540;
        retval = ioctl(fd_fb_1, FBIOPUT_VSCREENINFO, &fb_fg_var);
        if (retval < 0)
        {
                printf("FBIOPUT_VSCREENINFO error\n");
        }

		printf("\nfb_fg_var.xres=%d\n",fb_fg_var.xres);
		printf("\nfb_fg_var.yres=%d\n",fb_fg_var.yres);
		printf("\nfb_fg_var.xres_virtual=%d\n",fb_fg_var.xres_virtual);
		printf("\nfb_fg_var.yres_virtual=%d\n",fb_fg_var.yres_virtual);
		printf("\nfb_fg_var.bits_per_pixel=%d\n",fb_fg_var.bits_per_pixel);
	//new4-end
	
	//add-start
		retval = ioctl(fd_fb_1, FBIOBLANK, FB_BLANK_UNBLANK);
        if (retval < 0)
        {
                printf("FBIOBLANK errror\n");
        }

//		retval = ioctl(fd_fb_0, FBIOGET_VSCREENINFO, &screen_info);
//        if (retval < 0)
//        {
//                ;
//        }
//        printf("Set the background to 16-bpp\n");
//        screen_info.bits_per_pixel = 16;
//        screen_info.yoffset = 0;
//        retval = ioctl(fd_fb0, FBIOPUT_VSCREENINFO, &screen_info);
//        if (retval < 0)
//        {
//                ;
//        }
	//add-end

		//add5-start
		g_fb1_size = fb_fg_var.xres * fb_fg_var.yres_virtual * fb_fg_var.bits_per_pixel / 8;
		fb1 = (unsigned short *)mmap(0, g_fb1_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb_1, 0);
		if ((int)fb1 <= 0)
        {
                printf("\nError: failed to map framebuffer device 1 to memory.\n");
        }
//		memset(fb1, 0x00ff, g_fb1_size/2);
//		sleep(60);
		//add5-end

	
		
    

//			color_key.enable = 1;
//			//设置前景的色键颜色
//	        color_key.color_key = 0xFFFFFFFF; // black
////	        key.color_key = 0x00FF0000; // red
//	        ioctl(fb_device_0, MXCFB_SET_CLR_KEY, &color_key);

//			color_key.enable = 1;
//			//设置前景的色键颜色
//			color_key.color_key = 0xFFFFFFFF; // black
//			ioctl(fb_device_1, MXCFB_SET_CLR_KEY, &color_key);

			//设置背景层alpha
//			g_alpha.enable = 1;
//			//add1_start
//			g_alpha.alpha=0;
//			ioctl(fb_device_0, MXCFB_SET_GBL_ALPHA, &g_alpha);
//			ioctl(fb_device_0, MXCFB_WAIT_FOR_VSYNC, 0);


        

	if (strcmp(fb_fg_fix.id, "DISP3 BG - DI1") == 0)
		g_display_lcd = 1;
	else if (strcmp(fb_fg_fix.id, "DISP3 BG") == 0)
		g_display_lcd = 0;
	else if (strcmp(fb_fg_fix.id, "DISP4 BG") == 0)
		g_display_lcd = 3;
	else if (strcmp(fb_fg_fix.id, "DISP4 BG - DI1") == 0)
		g_display_lcd = 4;

	printf("fb_fg_fix.id=%s\n",fb_fg_fix.id);
	printf("g_display_lcd=%d\n",g_display_lcd);

        if (ioctl(fd_v4l, VIDIOC_S_OUTPUT, &g_display_lcd) < 0)
        {
                printf("VIDIOC_S_OUTPUT failed\n");
                return TFAIL;
        }
	printf("g_display_lcd=%d\n",g_display_lcd);


        fmt.type = V4L2_BUF_TYPE_VIDEO_OVERLAY;
        fmt.fmt.win.w.top=  g_display_top ;
        fmt.fmt.win.w.left= g_display_left;
        fmt.fmt.win.w.width=g_display_width;
        fmt.fmt.win.w.height=g_display_height;


		//配置摄像头的数据格式
        if (mxc_v4l_overlay_setup(&fmt) < 0) {
                printf("Setup overlay failed.\n");
                return TFAIL;
		}

        memset(&fb_v4l2, 0, sizeof(fb_v4l2));

	printf("g_fd_fb_fg=%d\n",g_fd_fb_fg);


		printf("g_overlay=%d\n",g_overlay);
        if (!g_overlay) {
//	        g_alpha.alpha = 255;
			g_alpha.alpha = 255;
	        g_alpha.enable = 1;
//			g_alpha.enable = 0;
	        if (ioctl(fd_fb_1, MXCFB_SET_GBL_ALPHA, &g_alpha) < 0) {
			printf("Set global alpha failed\n");
			close(fd_fb_1);//这里fb被关闭掉了?
			close(g_fd_fb_fg);
                        return TFAIL;
	        }

		printf("g_alpha_mode=%d\n",g_alpha_mode);




		fb_v4l2.fmt.width = fb_fg_var.xres;
		fb_v4l2.fmt.height = fb_fg_var.yres;
		printf("fb_fg_var.xres=%d\n",fb_fg_var.xres);
		printf("fb_fg_var.yres=%d\n",fb_fg_var.yres);
		

                if (fb_fg_var.bits_per_pixel == 32) 
					{
                        fb_v4l2.fmt.pixelformat = IPU_PIX_FMT_BGR32;
                        fb_v4l2.fmt.bytesperline = 4 * fb_v4l2.fmt.width;
                }
                else if (fb_fg_var.bits_per_pixel == 24) {
                        fb_v4l2.fmt.pixelformat = IPU_PIX_FMT_BGR24;
                        fb_v4l2.fmt.bytesperline = 3 * fb_v4l2.fmt.width;
                }
                else if (fb_fg_var.bits_per_pixel == 16) {
                        fb_v4l2.fmt.pixelformat = IPU_PIX_FMT_RGB565;
                        fb_v4l2.fmt.bytesperline = 2 * fb_v4l2.fmt.width;
                }

                fb_v4l2.flags = V4L2_FBUF_FLAG_PRIMARY;

				//这里应该是获取到了lcd framebuffer的地址信息给摄像头模块
                fb_v4l2.base = (void *) fb_fg_fix.smem_start +
				fb_fg_fix.line_length*fb_fg_var.yoffset;
        } 

		close(fd_fb_1);

		//配置摄像头的framebuffer
        if (ioctl(fd_v4l, VIDIOC_S_FBUF, &fb_v4l2) < 0)
        {
                printf("set framebuffer failed\n");
                return TFAIL;
        }

        if (ioctl(fd_v4l, VIDIOC_G_FBUF, &fb_v4l2) < 0) {
                printf("set framebuffer failed\n");
                return TFAIL;
        }

        printf("\n frame buffer width %d, height %d, bytesperline %d\n",
                fb_v4l2.fmt.width, fb_v4l2.fmt.height, fb_v4l2.fmt.bytesperline);
		ioctl(fd_v4l, VIDIOC_OVERLAY, &testoverlay);
//		sleep(10);
//        close(fd_v4l);
        return ret;
}

int colsoOv5642(void)
{
	int fd_fb0 = 0;
	int fd_fb1 = 0;
	int g_fb0_size=0;
	int g_fb1_size=0;
	unsigned short *fb0=0;
	unsigned short *fb1=0;
	int retval=0;
	char fb_device_0[100] = "/dev/fb0";
	char fb_device_1[100] = "/dev/fb1";
	struct fb_var_screeninfo screen_info;
    struct fb_fix_screeninfo fb_fix;
		
	close(fd_v4l);//先关闭摄像头

	//再次打开framebuffer
	if ((fd_fb1 = open(fb_device_1, O_RDWR )) < 0)//打开fb1
	{
		printf("Unable to open frame buffer 0\n");
        return -1;
    }

	retval = ioctl(fd_fb1, FBIOGET_VSCREENINFO, &screen_info); //获取fb1可变信息
        if (retval < 0)
        {
                printf("**\n");
        }
		
//	printf("Set the background to 24-bpp\n");
	screen_info.bits_per_pixel = 16;
    screen_info.yoffset = 0;
    retval = ioctl(fd_fb1, FBIOPUT_VSCREENINFO, &screen_info);
	if (retval < 0)
        {
                printf("***\n");
        }
	
	g_fb1_size = screen_info.xres * screen_info.yres_virtual * screen_info.bits_per_pixel / 8;

	fb1 = (unsigned short *)mmap(0, g_fb1_size,PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb1, 0);
    if ((int)fb1 <= 0)
    {
          printf("\nError: failed to map framebuffer device 0 to memory.\n");
    }

	//用黑色填充framebuffer
	memset(fb1, 0x0, g_fb1_size);
	close(fd_fb1);
	
}



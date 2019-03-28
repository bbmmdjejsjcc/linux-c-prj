
CROSS_COMPILE = arm-linux-gnueabihf-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

#CFLAGS := -Wall -Werror -O2 -g
CFLAGS := -O2 -g
CFLAGS += -I $(shell pwd)/include
CFLAGS += -I /my-imx6/02_source/yibiao/alsa/alsa-lib-1.0.28/build/include
#CFLAGS += -I/my-imx6/05_rootfs/rootfs/myzr-rootfs/3.14.52/rootfs-qt5/aplay/alsa-lib-1.0.28/build/include

#LDFLAGS := -lm -lfreetype -lts -lpthread -ljpeg
LDFLAGS := -lm  -lpthread 
LDFLAGS += -L/my-imx6/02_source/yibiao/alsa/alsa-lib-1.0.28/build/lib -lasound

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := app1


obj-y += main.o
obj-y += ov5642/
#obj-y += tdcan/
obj-y += local_socket/
obj-y += update/
obj-y += wm8962/
#obj-y += wm8962/play/
#obj-y += wm8962/setVolume/
#obj-y += fonts/
#obj-y += input/
#obj-y += debug/
#obj-y += render/
#obj-y += page/
#obj-y += file/

all : 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o


clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	
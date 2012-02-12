#CC      = gcc
#
#CFLAGS  = -I. -I/usr/src/linux-headers-3.1.0-1-common/include
#CFLAGS += -Wall
#CFLAGS += -O2
#CFLAGS += -std=c99
#
#TARGET    = senorusb
#BUILD_DIR = build
SRC_DIR   = src
#
#SRC = $(TARGET).c
#
#OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)
#
#
#all: $(TARGET)
#
#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) $^ -o $@
#
#$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
#	@mkdir -p $(BUILD_DIR)
#	$(CC) -c $(CFLAGS) $< -o $@
#
#clean:
#	rm -f $(OBJS)
#	rm -f $(target)

obj-m = senorusb.o
KVERSION = $(shell uname -r)
all: senorusb.c
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

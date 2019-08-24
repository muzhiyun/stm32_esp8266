#ifndef _DISPLAY_JPG_H_
#define _DISPLAY_JPG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>

int display_jpg(char * filename, unsigned int *lcd_ptr,unsigned int x_s, unsigned int y_s, unsigned int fb_width,unsigned int fb_height,unsigned int fb_depth);

#endif
#ifndef PAINTER_H
#define PAINTER_H
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define u_int32_t unsigned int



enum Color {Color_red = 0x00FF0000,
           Color_green = 0x0000FF00,
           Color_blue = 0x000000FF,
           Color_white = 0,
           Color_black = 0x00FFFFFF,
           Color_gray = 171 << 16 | 171 << 8 | 171};

extern int painterInit(void);
extern void fillBackground(u_int32_t color);
extern void paintPoint(int x,int y,u_int32_t color);

#endif // PAINTER_H


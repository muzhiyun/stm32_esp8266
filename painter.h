#ifndef PAINTER_H
#define PAINTER_H
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
// #include "asc8x16e.h"
// #include "chinese16x16e.h"

#define u_int32_t unsigned int
//#define DEBUG

typedef enum Color {Color_red = 0x00FF0000,
           Color_green = 0x0000FF00,
           Color_blue = 0x000000FF,
           Color_white = 0,
           Color_black = 0x00FFFFFF,
           Color_gray = 171 << 16 | 171 << 8 | 171}Color;

extern int painterInit(void);
extern void fillBackground(u_int32_t color);
extern void paintPoint(int x, int y, Color color);
extern void paint_H_line(int x0, int y0, int x1, Color color);
extern void paint_V_line(int x0, int y0, int y1, Color color);
extern void paintLine(int x1, int y1, int x2, int y2, Color color);
extern void paintRect(int x, int y, int w, int h, Color color);
extern void fillRect(int x, int y, int w, int h, Color color);
extern void paintCircle(int x, int y, int r, Color color);
extern void fillCircle(int x, int y, int r, Color color);
extern void paintBMP(int x, int y,const char *fileName);
extern void paintAsc(int x, int y, Color tc, Color bc, int index);
extern void printChinese(int x, int y, Color tc, Color bc, int index);
#endif // PAINTER_H


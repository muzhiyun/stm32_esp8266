#ifndef _DISPLAY_JPEG_H_
#define _DISPLAY_JPEG_H_

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <jpeglib.h>
#include <jerror.h>
#define u_int32_t unsigned int


extern int display_jpeg (char * filename, unsigned int *lcd_ptr,unsigned int x_s, unsigned int y_s);
#endif
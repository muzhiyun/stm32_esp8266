#include "painter.h"

#define FBFILENAME "/dev/fb0"

u_int32_t width, height;
u_int32_t *fbp;

int painterInit(void)
{
    int fbfd = open("/dev/fb0", O_RDWR);

    if(-1 == fbfd)
    {
        perror("error!");
        return -1;
    }
    struct fb_fix_screeninfo fInfo;
    ioctl(fbfd, FBIOGET_FSCREENINFO, &fInfo);

    printf("smem_len = %u,line_length =  %u\n", fInfo.smem_len, fInfo.line_length);

    struct fb_var_screeninfo vInfo;
    ioctl(fbfd, FBIOGET_VSCREENINFO, &vInfo);
    printf("xres = %u, yres = %u, xres_virtual = %u, yres_virtual = %u"
           "xoffset = %u, yoffset = %u, bits_per_pixel = %u\n", vInfo.xres, vInfo.yres, vInfo.xres_virtual, vInfo.yres_virtual, vInfo.xoffset, vInfo.yoffset, vInfo.bits_per_pixel);
    width = vInfo.xres_virtual;
    height = vInfo.yres_virtual;
    fbp = (unsigned int *)mmap(NULL, fInfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    return 0;
}

void fillBackground(u_int32_t color)
{
    u_int32_t i, j;
    u_int32_t *q = fbp;
    for(i = 0;i < height;++i)
    {
        for(j = 0;j < width;++j)
        {
            *q++ = color;
        }
    }
}


void paintPoint(int x,int y,u_int32_t color)
{
    u_int32_t(*p)[width] = (u_int32_t (*)[width])fbp;
    *(*(p+y)+x) = color ;

}

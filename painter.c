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

void paintPoint(int x, int y, Color color)
{
    if(x < 0 || y < 0 || (u_int32_t)x >= width || (u_int32_t)y >= height)
    {
        return ;
    }
    u_int32_t (*p)[width] = (u_int32_t (*)[width])fbp;
    p[y][x] = color;
}

void swap(int *p1, int *p2)
{
    int t = *p1;
    *p1 = *p2;
    *p2 = t;
}

void paint_H_line(int x0, int y0, int x1, Color color)
{
    int i;
    if(x0 > x1)
    {
        swap(&x0, &x1);
    }
    for(i = x0;i <= x1;++i)
    {
        paintPoint(i, y0, color);
    }
}

void paint_V_line(int x0, int y0, int y1, Color color)
{
    if(y0 > y1)
    {
        swap(&y0, &y1);
    }
    int i;
    for(i = y0;i <= y1;++i)
    {
        paintPoint(x0, i, color);
    }
}

void paintLine(int x1, int y1, int x2, int y2, Color color)
{
    if(y1 == y2)
    {
        paint_H_line(x1, y1, x2, color);
        return ;
    }
    else if(x1 == x2)
    {
        paint_V_line(x1, y1, y2, color);
        return ;
    }
    double k = (y2 - y1) / (x2 - x1);
    int i;

    if(x1 > x2)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    for(i = x1;i <= x2;++i)
    {
        int y;
        y = k * i - k * x1 + y1;
        paintPoint(i, y, color);
    }
}

void paintRect(int x, int y, int w, int h, Color color)
{
    paintLine(x, y, x + w, y, color);
    paintLine(x + w, y, x + w, y + h, color);
    paintLine(x, y + h, x + w, y + h, color);
    paintLine(x, y , x , y + h, color);
}

void fillRect(int x, int y, int w, int h, Color color)
{
    int i;

    for(i = y;i <= y + h;++i)
    {
        paintLine(x, i, x + w, i, color);
    }
}

void paintCircle(int x, int y, int r, Color color)
{
    double d;
    for(d = 0;d < 360;++d)
    {
        double dd;
        dd = d / 360 * 2 * 3.14159265;
        int xx = (int)(r * cos(dd) + x);
        int yy = (int)(r * sin(dd) + y);
        paintPoint(xx, yy, color);
    }
}

void fillCircle(int x, int y, int r, Color color)
{

    int i;
    for(i =  y - r;i <= y + r;++i)
    {
        double d;
        d = asin((double)(i - y) / r);
        double k = r * cos(d);
        paintLine(k + x, i, x - k, i, color);
    }
}

void paintBMP(int x, int y,const char *fileName)
{
    struct stat st;
    int ret = stat(fileName, &st);
    if(ret == -1)
    {
        perror("stat error!");
        return ;
    }
    char *p = malloc(st.st_size);

    int fd = open(fileName, O_RDONLY);
    if(fd == -1)
    {
        perror("open error!");
        return ;
    }

    read(fd, p, st.st_size);

    int offset;
    offset = *(int *)(p + 10);
#ifdef DEBUG
    printf("offset = %d\n", offset);
#endif
    int w = *(int *)(p + 0x12);
    int h = *(int *)(p + 0x16);
#ifdef DEBUG
    printf("w = %d,h = %d\n", w, h);
#endif

    char *q = p + offset;

    int i, j;
    for(i = h - 1;i >= 0;--i)
    {
        for(j = 0;j < w;++j)
        {
            char r;
            char g;
            char b;
            r = *q++;
            g = *q++;
            b = *q++;
            paintPoint(x + j, y + i, b << 16 | g << 8 | r);
        }
    }

    free(p);
    close(fd);
}
/*
void paintAsc(int x, int y, Color tc, Color bc, int index)
{
    unsigned char *p = nAsciiDot + (index * 16);

    int i;
    for(i = 0;i < 16;++i)
    {
        unsigned char c = *p++;
        int j;
        for(j = 0;j < 8;++j)
        {
            if(c & 0x80)
            {
                paintPoint(x + j, y + i, tc);
            }
            else
            {
                paintPoint(x + j, y + i, bc);
            }
            c <<= 1;
        }
    }

}

void printChinese(int x, int y, Color tc, Color bc, int index)
{
    unsigned char *p = GB_16 + (index * 32);

    int i, j;
    for(i = 0;i < 16;++i)
    {
        unsigned char c = *p++;
        for(j = 0;j < 8;++j)
        {
            paintPoint(x + j, y + i, c & 0x80 ? tc : bc);
            c <<= 1;
        }
        c = *p++;
        for(;j < 16;++j)
        {
            paintPoint(x + j, y + i, c & 0x80 ? tc : bc);
            c <<= 1;
        }
    }
}
*/
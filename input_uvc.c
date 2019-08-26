#include "input_uvc.h"
#include "display_jpg.h"
#include "http_post.h"
#include "painter.h"

extern u_int32_t *fbp;


static int width,height;

int uvc_Init()
{

    int fdUsbCam = open("/dev/video15",O_RDWR); 
    if(-1 == fdUsbCam)
    {
        perror("uvc_Init open error");
        return -1;
    }
    struct v4l2_capability cap;
    int ret = ioctl(fdUsbCam, VIDIOC_QUERYCAP, &cap);
    if(ret != 0)
    {
        perror("ioctl VIDIOC_QUERYCAP");
        return -1;
    }

    /* 判断改设备支不支持捕获图像和流输出功能 */
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
        printf("it's camer!\n");
    else {
        printf("it's not a camer!\n");
        return -1;
    }
    if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
        printf("it's stream device!\n");
    else {
        printf("it's not a stream device!\n");
        return -1;
    }

    printf("Driver Name : %s\nCard Name : %s\nBus info : %s\nDriver Version : %u.%u.%u\n ",\
    cap.driver, cap.card, cap.bus_info,\
     (cap.version>>16)&0xff, (cap.version>>8)&0xff, (cap.version)&0xff);

    struct v4l2_format fmt;
    
    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fdUsbCam, VIDIOC_G_FMT, &fmt) < 0)
    {
        printf("get format failed\n");
        return -1;
    }
    //printf("h:%d w:%d\n",fmt.fmt.pix.height,fmt.fmt.pix.width);

    width = fmt.fmt.pix.width;
    height = fmt.fmt.pix.height;

    printf("width : %d  height : %d\n\
    pixelformat : %d\n\
    field : %d\n\
    bytesperline : %d\n\
    sizeimage : %d\n\
    colorspace : %d\n\
    priv : %d\n",\
    fmt.fmt.pix.width,\
     fmt.fmt.pix.height,\
    fmt.fmt.pix.pixelformat,\
     fmt.fmt.pix.field, \
     fmt.fmt.pix.bytesperline, \
     fmt.fmt.pix.sizeimage, \
     fmt.fmt.pix.colorspace, \
     fmt.fmt.pix.priv);
    /* 得到摄像头所支持的所有格式 */
    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format : \n");
    while (ioctl(fdUsbCam, VIDIOC_ENUM_FMT, &fmtdesc) != -1) {
        printf("\t%d.%s\n", fmtdesc.index+1, fmtdesc.description);
        fmtdesc.index++;
    }
    return fdUsbCam;
}


int imgGet(int fdUsbCam)
{

    //设置图像帧格式和像素大小

    struct v4l2_format fmt;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width  = 640;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG; //使用JPEG格式帧，用于静态图像采集
    ioctl(fdUsbCam, VIDIOC_S_FMT, &fmt);
    
    //读取一帧JPEG图像并写入文件

    void *buff = malloc(1024 * 1024);  //分配缓存足以容纳一帧图像
    unsigned int size = 0;
    //char name[10]={"0123456789"};
    //char filename[10];
    //int i = 0;
    int filefd = -1;
    //for(i=0;i<10;i++)
    //{
    //    size = read(fdUsbCam, buff, 1024 * 1024);   //检测是否和次数有关
    //}                                             
    size = read(fdUsbCam, buff, 1024 * 1024);       
    sleep(3);                                       //确定　和读取时间有关
    size = read(fdUsbCam, buff, 1024 * 1024);
    //sprintf(filename,"%c.jpg",name[i]);
    //filefd = open(filename, O_RDWR | O_CREAT,0666);
    filefd = open("image 22.jpg", O_RDWR | O_CREAT,0666);
    write(filefd, buff, size);
    close(filefd);

    
    free(buff);
    //printf("%d.jpg finish\n",i);
    //sleep(1);
    //puts("finish 3\n");
}

void videoLCD(int fdUsbCam,int connfd)
{
    int size = -1;
    void *buff = malloc(1024 * 1024);  //分配缓存足以容纳一帧图像
    int filefd = -1 ;

    while (1)
    {
        paintBMP(0,208,"left.bmp");
        paintBMP(744,208,"right.bmp");
        size = read(fdUsbCam, buff, 1024 * 1024);
        filefd = open("temp.jpg", O_RDWR | O_CREAT,0666);
        write(filefd, buff, size);
        close(filefd);
        display_jpg("temp.jpg",fbp,80,0,800,480,32);
        send_data("temp.jpg",connfd);
        puts("send finish");
        sleep(1);
    }
    free(buff);

}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <pthread.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <syslog.h>
#include <linux/types.h>  

#include "input_uvc.h"
#include "painter.h"
#include "display_jpeg.h"

extern u_int32_t *fbp;;

int main(int argc, char *argv[])
{
    painterInit();
    int fd=uvc_Init();
    imgGet(fd);
    //display_jpeg("./0.jpg",fbp,100,100);
}
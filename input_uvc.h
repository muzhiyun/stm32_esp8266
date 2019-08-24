#ifndef _INPUT_UVC_H_
#define _INPUT_UVC_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <pthread.h>
#include <syslog.h>
#include <fcntl.h>

#include <linux/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>


extern int uvc_Init();
extern int imgGet(int fdUsbCam);
extern void videoLCD(int fdUsbCam,int connfd);
#endif
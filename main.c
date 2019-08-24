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
#include "display_jpg.h"
#include "http_post.h"


extern u_int32_t *fbp;
extern u_int32_t width, height;

int main(int argc, char *argv[])
{
    
    painterInit();
    fillBackground(Color_black);
    int fd=uvc_Init();
    imgGet(fd);
    int connfd = socket_init();
    puts("sockfd OK");

    // int ret = fork();
    // if(ret == -1)
    // {
    //     perror("fork");
    //     exit(1);
    //     }
    // if (pid == 0)
    // {
    
    videoLCD(fd,connfd);
    
    /*
    send_data("temp.jpg",connfd);
    puts("send finish");
    close(connfd);
    */
    // }
    // else
    // {
    //     while (1)
    //     {
        
    //     }
        
    //}
    
    
    

    //printf("main %d  %d\n",height,width);
    //display_jpg("image.jpg",fbp,100,100,width,height,32);
}
#ifndef _HTTP_POST_H_
#define _HTTP_POST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/socket.h>
#include<error.h>
#include<pthread.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>

extern int socket_init();
extern int send_data(char *filename,int connfd);

#endif
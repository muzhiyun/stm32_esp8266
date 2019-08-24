#include "http_post.h"

#define SERVER "*.*.*.*"
//#define SERVER "192.168.2.166"

int socket_init()
{
    int connfd = socket(AF_INET,SOCK_STREAM,0);
    if(connfd == -1)
    {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(80);
	serAddr.sin_addr.s_addr = inet_addr(SERVER);

    int ret = connect(connfd,(struct sockaddr *)&serAddr,sizeof(serAddr));
    if (-1 == ret)
    {
        perror("Network failed!");
        exit(1);
    }
    return connfd;
}

int send_data(char *filename,int connfd)
{
    struct stat st;
    struct tm *p;			//时间结构体

    int filefd = -1;
    char buff[4096] = {0};
    //void *buff = malloc(1024 * 1024);  //分配缓存足以容纳一帧图像
    int ret = -1;

    if ((filefd = open(filename, O_RDONLY)) < 0) {
        perror("open");
        exit(-1);
    }

    if (fstat(filefd, &st) < 0) {
        perror("fstat");
        exit(-1);
    }

    char host[48];
    char len[20];
    char filenm[512];
    char *cmd[15];
    struct timeval tv;
    gettimeofday(&tv,NULL);


    cmd[9] = "------WebKitFormBoundaryd6NfgrZ24b1aKMpS\r\n";
    
    cmd[11] = "Content-Type: image/jpeg\r\n";
    cmd[12] = "\r\n";
    char *end = "\r\n------WebKitFormBoundaryd6NfgrZ24b1aKMpS--\r\n";


    //sprintf(host,"Host: %s\r\n",SERVER);
    sprintf(filenm,"Content-Disposition: form-data; name=\"file\"; filename=\"%lu.jpg\"\r\n",tv.tv_sec);
    sprintf(len,"Content-Length: %lu\r\n",st.st_size+strlen(cmd[9])+strlen(filenm)+strlen(cmd[11])+strlen(cmd[12])+strlen(end));
    cmd[0] = "POST /test/post.php HTTP/1.1\r\n";
    cmd[1] = "Host: *.*.*.*\r\n";  //HOST
    cmd[2] = "Connection: keep-alive\r\n";
    cmd[3] = len;
    //cmd[3] = "Content-Length: 31412\r\n";
    //31412 31233 179
    //39018 38839 
    //cmd[4] = "Cache-Control: no-cache\r\n";
    cmd[4] = "Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryd6NfgrZ24b1aKMpS\r\n";
    //cmd[5] = "Content-Type: application/octet-stream\r\n";
    cmd[5] = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\n";
    cmd[6] = "Accept-Encoding: gzip, deflate\r\n";
    //cmd[7] = "Transfer-Encoding:null\r\n";
    cmd[7] = "Accept-Language: en-US,en;q=0.9,zh-CN;q=0.8,zh-TW;q=0.7,zh;q=0.6\r\n";
    cmd[8] = "\r\n";
    cmd[10] = filenm;
    

    time_t t;
	time(&t);


    puts("-----------------------");
    printf("%s",cmd[1]);
    printf("%s",cmd[3]);
    printf("%s",cmd[10]);
    printf("%s",ctime(&t));
    printf("time:%lu\n",tv.tv_sec);
    puts("-----------------------");
    int i = 0;
    for(i=0;i<13;i++)
    {
        send(connfd,cmd[i],strlen(cmd[i]),0);
    }

    puts("send head OK");
    //printf("%d\n",ret);
    while(1)
    {
        ret=read(filefd,buff,4096);
        if(ret == 0)
        {
            break;
        }
        send(connfd,buff,ret,0);
        //printf("in %d\n",ret);
    }
    //printf("%d\n",ret);
    puts("send body OK");
    

    send(connfd,end,strlen(end),0);
    puts("send tail OK");
    memset(buff,0,sizeof(buff));
    //close(connfd);
    recv(connfd,buff,4096,0);
    printf("recv:%s",buff);
    puts("---------------------------");
    close(filefd);
    return 0;
    
}
#include "http_post.h"

#define SERVER "123.207.155.16"
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
    char *cmd[25];
    int i = 0;
    int body_length = 0;
    struct timeval tv;
    gettimeofday(&tv,NULL);

    time_t t;
	time(&t);

    /*模拟GPS数据*/
/*    //根据时间戳生成可变的坐标
    //1566740890
    //108.947058
    // 34.344497
    // 

    // 10
    // 108.947116
    // 34.344341                //1566703626  //3634.
    gettimeofday(&tv,NULL);
    double temp_time = (tv.tv_sec-1566740000);
    temp_time = temp_time/100000;
    printf("%lf\n",temp_time);

    
    char x[16]={0};
    sprintf(x,"%.8f\r\n",(temp_time+108.947687-0.42702));
    char y[16]={0};
    sprintf(y,"%.8f\r\n",(temp_time+34.344126-0.42702));
    char h[16]={0};
    sprintf(h,"%.8f\r\n",(temp_time+88.87));
    //108.951137 34.347576
    //x:109.27015700  y:34.66659600
*/
    FILE *fp;
	fp = fopen("gps.data", "r");
	if (fp == NULL){
		printf("Open File for read failed.\n");
		exit(0);
	}
	double xd,yd,hd;
	while (!feof(fp)){
		fscanf(fp, "%lf%lf%lf", &xd, &yd, &hd);
	}
    char x[16]={0};
    sprintf(x,"%.8f\r\n",(xd+0.00022));
    char y[16]={0};
    sprintf(y,"%.8f\r\n",(yd+0.00022));
    char h[16]={0};
    sprintf(h,"%.8f\r\n",(hd+0.87));
	fclose(fp);

    fp = fopen("gps.data", "w");
	if (fp == NULL){
		printf("Open File for write failed.\n");
		exit(0);
	}
    fprintf(fp, "%s%s%s", &x, &y, &h);
    fclose(fp);

    cmd[9] = "------WebKitFormBoundaryd6NfgrZ24b1aKMpS\r\n";                        //为了计算Content-Length　先规划所有请求体，再设计请求头
    cmd[10] = "Content-Disposition: form-data; name=\"x\"\r\n";
    cmd[11] = "\r\n";
    cmd[12] = x;//"108.947687\r\n";          //x
    cmd[13] = "------WebKitFormBoundaryd6NfgrZ24b1aKMpS\r\n";
    cmd[14] = "Content-Disposition: form-data; name=\"y\"\r\n";
    cmd[15] = "\r\n";
    cmd[16] = y;//"34.344126\r\n";          //y
    cmd[17] = "------WebKitFormBoundaryd6NfgrZ24b1aKMpS\r\n";
    cmd[18] = "Content-Disposition: form-data; name=\"h\"\r\n";
    cmd[19] = "\r\n";
    cmd[20] = h;//"88\r\n";          //h
    cmd[21] = "------WebKitFormBoundaryd6NfgrZ24b1aKMpS\r\n";                    
            sprintf(filenm,"Content-Disposition: form-data; name=\"file\"; filename=\"%lu.jpg\"\r\n",tv.tv_sec);  //cmd[10]
    cmd[22] = filenm;
    cmd[23] = "Content-Type: image/jpeg\r\n";
    cmd[24] = "\r\n";
    

    char *end = "\r\n------WebKitFormBoundaryd6NfgrZ24b1aKMpS--\r\n";       //end为最后发送的头
    
    
    for (i=9;i<25;i++)          //统计body长度
    {
        body_length += strlen(cmd[i]);
    }


    sprintf(len,"Content-Length: %lu\r\n",st.st_size+body_length+strlen(end));  //cmd[3]

    cmd[0] = "POST /test/post.php HTTP/1.1\r\n";
    cmd[1] = "Host: 123.207.155.16\r\n";  //HOST
    cmd[2] = "Connection: keep-alive\r\n";
    cmd[3] = len;
    cmd[4] = "Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryd6NfgrZ24b1aKMpS\r\n";
    cmd[5] = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\n";
    cmd[6] = "Accept-Encoding: gzip, deflate\r\n";
    cmd[7] = "Accept-Language: en-US,en;q=0.9,zh-CN;q=0.8,zh-TW;q=0.7,zh;q=0.6\r\n";
    cmd[8] = "\r\n";
    


    puts("-----------------------");
    printf("%s",cmd[1]);        //host
    printf("%s",cmd[3]);        //Content-Length
    printf("x:%s",cmd[12]);       //x
    printf("y:%s",cmd[16]);       //y
    printf("h:%s",cmd[20]);       //h
    printf("%s",cmd[21]);         //filename
    printf("%s",ctime(&t));     //正常时间
    printf("time:%lu\n",tv.tv_sec); //时间戳
    puts("-----------------------");
    // close(connfd);
    // exit(0);
    for(i=0;i<25;i++)
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
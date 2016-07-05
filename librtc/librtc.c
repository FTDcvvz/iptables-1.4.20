#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ruletables.h"

#define CLIENTPORT 3333
#define BUFFER_SIZE 1024

struct rtc_handle{
	struct list_head * head;
};

struct rtc_handle *
RTC_INIT(){
	int    sockfd, n;
    rtc_handle * rthandle = NULL;
    struct sockaddr_in    servaddr;

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
	    exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(CLIENTPORT);
    if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
    printf("inet_pton error for %s\n",argv[1]);
    exit(0);
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }

    //recv chain head
    int needRecv=sizeof(struct list_head);
    char *buffer=(char*)malloc(needRecv);
    rthandle=(rtc_handle*)malloc(sizeof(rtc_handle));
    int pos=0;
    int len;
    while(pos < needRecv)
    {
        len = recv(sockfd, buffer+pos, BUFFER_SIZE, 0);
        if (len < 0)
        {
            printf("Client Recieve Data Failed!\n");
            break;
        }
        pos+=len;

    }
    close(sockfd);
    memcpy(rthandle->head,buffer,needRecv);
    free(buffer);
    
    return rthandle;
}

void 
RTC_FREE(struct rtc_handle *h){
	free (h);
}

int 
RTC_SET_POLICY(const char * policy, struct rtc_handle *handle){
	printf("%s\ntest succeed\n",policy);
	return 1;
}


int 
RTC_APPEND(const STRUCT_ENTRY *e, 
		struct rtc_handle *handle)
{
	return 1;
}



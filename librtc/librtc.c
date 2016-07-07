#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ruletables.h"

#define CLIENTPORT 3333
#define BUFFER_SIZE 10

#define SET_POLICY 0
#define APPEND 1

struct rtc_handle{
	int command;
	ruletable table;
};

int
RTC_INIT(){
	int    sockfd, n;
    struct sockaddr_in    servaddr;

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
	    exit(0);
	}
	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(CLIENTPORT);
    if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
    printf("inet_pton error\n");
    exit(0);
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }

    return 1;
    
}

int 
RTC_SET_POLICY(const char * chain ,const char * policy,const char * tablename){	


    int needSend=sizeof(struct rtc_handle); //recv a pointer (list_head *)
		//(e.s ,32bit computer:  4Byte ,64bit computer : 8Byte)
    char *buffer=(char*)malloc(needSend);
    struct rtc_handle* rthandle = (struct rtc_handle*)malloc(needSend);
    //build rthandle
    rthandle->command = SET_POLICY;
    strcpy(rthandle->table.actionType,chain);
    strcpy(rthandle->table.property.tablename,tablename);
    strcpy(rthandle->table.property.policy,policy);

    int pos=0;
    int len;
    memcpy(buffer,rthandle,needSend);
    while(pos < needSend)
    {
        len = send(sockfd, buffer+pos, BUFFER_SIZE, 0);
        if (len < 0)
        {
            printf("Send Data Failed!\n");
            break;
        }
        pos+=len;
    }
    printf("policy send succeed\n");

    close(sockfd);
    free(buffer);
    free(rthandle);
    buffer = NULL;
    rthandle = NULL;
	return 1;
}


int 
RTC_APPEND(const STRUCT_ENTRY *e)
{
	return 1;
}



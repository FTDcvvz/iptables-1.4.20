#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>  
#include "ruletables.h"

#define BUFFER_SIZE 10

#define SET_POLICY 0
#define APPEND 1

#define UNIX_DOMAIN "/tmp/UNIX.domain" 

struct rtc_handle{
	int command;
	ruletable table;
};

int 
RTC_SET_POLICY(const char * chain ,const char * policy,const char * tablename){	
int    sockfd;
    struct sockaddr_un    servaddr;

	if( (sockfd = socket(PF_UNIX,SOCK_STREAM,0)) < 0){
	    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
	    exit(0);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sun_family=AF_UNIX;  
    strcpy(servaddr.sun_path,UNIX_DOMAIN);  

    if( connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0){
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }

    int needSend=sizeof(struct rtc_handle); //recv a pointer (list_head *)
		//(e.s ,32bit computer:  4Byte ,64bit computer : 8Byte)
    char *buffer=(char*)malloc(needSend);
    struct rtc_handle* rthandle = (struct rtc_handle*)malloc(needSend);
    //build rthandle
    rthandle->command = SET_POLICY;
    strcpy(rthandle->table.actionType,chain);
    strcpy(rthandle->table.property.tablename,tablename);
    strcpy(rthandle->table.actionDesc,policy);

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



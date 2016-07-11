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

#define UNIX_DOMAIN "/tmp/UNIX.domain" 

typedef enum _command_list
{	SET_POLICY ,
	APPEND	,
	INSERT ,
	DELETE ,
	CLEAN  ,
	ALLIN		
}command_list;
struct handle{
    command_list command;
    ruletable table;
}; 
static int conn()
{
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
	return sockfd;
}

int 
RTC_SET_POLICY(const char * chain ,const char * policy,const char * tablename){	
    int sockfd = conn();
    int needSend=sizeof(struct rtc_handle);
    char *buffer=(char*)malloc(needSend);
    struct rtc_handle* rthandle = (struct rtc_handle*)malloc(needSend);
    //build rthandle
    
    rthandle->command = SET_POLICY;

    if(strcmp(chain,"INPUT") == 0)
        rthandle->table.actionType = INPUT;
    else if(strcmp(chain,"OUTPUT") == 0)
        rthandle->table.actionType = OUTPUT;
    else if(strcmp(chain,"FORWARD") == 0)
        rthandle->table.actionType = FORWARD;
    else if(strcmp(chain,"PREROUTING") == 0)
        rthandle->table.actionType = PREROUTING;
    else if(strcmp(chain,"POSTROUTING") == 0)
        rthandle->table.actionType = POSTROUTING;

    if(strcmp(policy,"ACCEPT") == 0)
        rthandle->table.actionDesc = ACCEPT;
    else if(strcmp(policy,"DROP") == 0)
        rthandle->table.actionDesc = DROP;
    else if(strcmp(policy,"QUEUE") == 0)
        rthandle->table.actionDesc = QUEUE;
    else if(strcmp(policy,"RETURN") == 0)
        rthandle->table.actionDesc = RETURN;
	

    if(strcmp(tablename,"filter") == 0)
        rthandle->table.property.tablename = filter;
    else if(strcmp(tablename,"nat") == 0)
        rthandle->table.property.tablename = nat;
    else if(strcmp(tablename,"mangle") == 0)
        rthandle->table.property.tablename = mangle;

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



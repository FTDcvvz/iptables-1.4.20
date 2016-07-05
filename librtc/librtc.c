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

static ruletable * find_policy_place(const char * chain ,const char * tablename, struct rtc_handle *handle)
{
	//find tablename(filter nat mangle)
	struct list_head * plist;
	ruletable * rt;
	int flag = 0;
	list_for_each(plist,handle->head){
		rt = list_entry(plist,ruletable,list);
		if(strcmp(rt->property.tablename,tablename) == 0)
			break; 
	}
	//find chain(input output forward...)
	list_for_each(plist,(rt->list).prev){
		rt = list_entry(plist,ruletable,list);
		if(strcmp(rt->actionType,chain)==0)
			flag = 1;
		else {
			if(flag == 1)   //we'v got to the end of the "chain"(input/output/forward)
				break;
		}		
	}
	//set policy at the right place 
	plist = (rt->list).prev;
	rt = list_entry(plist,ruletable,list);
} 

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
RTC_SET_POLICY(const char * chain ,const char * policy,const char * tablename, struct rtc_handle *handle){
	
	rultable * rt ;
	rt = find_policy_place(chain,tablename,handle);
	rt->property = {tablename,policy};
	rt->actionType = chain;

	return 1;
}


int 
RTC_APPEND(const STRUCT_ENTRY *e, 
		struct rtc_handle *handle)
{
	return 1;
}



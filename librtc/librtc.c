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
		printf("%s\n",rt->property.tablename);
		if(strcmp(rt->property.tablename,tablename) == 0)
			break; 
	}
	//find chain(input output forward...)
	list_for_each(plist,(rt->list).prev){
		rt = list_entry(plist,ruletable,list);
		printf("%s\n",rt->actionType);
		if(strcmp(rt->actionType,chain)==0)
			flag = 1;
		else {
			if(flag == 1)   //we'v got to the end of the "chain"(input/output/forward)
				break;
		}		
	}
	//find policy at the right place 
	plist = (rt->list).prev;
	rt = list_entry(plist,ruletable,list);
	printf("%s\n",rt->property.policy);
	return rt;
} 

struct rtc_handle *
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

    //recv chain head
    int needRecv=sizeof(struct rtc_handle); //recv a pointer (list_head *)
		//(e.s ,32bit computer:  4Byte ,64bit computer : 8Byte)
    char *buffer=(char*)malloc(needRecv);
    struct rtc_handle * rthandle = (struct rtc_handle*)malloc(needRecv);
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
    memcpy(rthandle,buffer,needRecv);
    close(sockfd);
    free(buffer);
    return rthandle;
}

void 
RTC_FREE(struct rtc_handle *h){
	free (h);
}

int 
RTC_SET_POLICY(const char * chain ,const char * policy,const char * tablename, struct rtc_handle *handle){	
	ruletable * rt = NULL;
	if((rt = find_policy_place(chain,tablename,handle)) == NULL){
		printf("find policy place error\n");
		exit(0);
	}	
	rt->property.policy = policy;
	printf("here we are,%s\n",rt->property.policy);
	return 1;
}


int 
RTC_APPEND(const STRUCT_ENTRY *e, 
		struct rtc_handle *handle)
{
	return 1;
}



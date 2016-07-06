#ifndef _ruletables_h
#define _ruletables_h
#include "linux_list.h"

typedef struct basic_header
{
	u_int32_t s_addr,d_addr;
	u_int32_t smsk,dmsk;
	u_int16_t spts[2];
	u_int16_t dpts[2]; 
}basic_header;

typedef struct properties
{
	char * tablename;
	
	char * policy;
}properties;

typedef struct ruletable
{
	struct list_head list;

	basic_header head;

	int priority;
	char * actionType;
	char * actionDesc;

	properties property;
}ruletable;
#endif

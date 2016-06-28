
struct rtc_handle{

};

struct rtc_handle *
RTC_INIT(){

}

void 
RTC_FREE(struct rtc_handle *h){

}

int 
RTC_SET_POLICY(const char * policy, struct rtc_handle *handle){
	printf("%s\ntest succeed\n",policy);
	return 1;
}

int 
RTC_COMMIT(struct rtc_handle *handle){
	return 1;
}

int 
RTC_APPEND(const STRUCT_ENTRY *e, 
		struct rtc_handle *handle)
{
	return 1;
}



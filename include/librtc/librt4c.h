#ifndef _LIBRT4C_H
#define _LIBRT4C_H



/* Take a snapshot of the rules.  Returns NULL on error. */
struct rtc_handle *rtc_init(const char *tablename);

/* Cleanup after rtc_init(). */
void rtc_free(struct rtc_handle *h);

/* Sets the policy */
int rtc_set_policy(const char * policy, 
		struct rtc_handle *handle);

/* Makes the actual changes. */
int rtc_commit(struct rtc_handle *handle);

/* Append to chain  */
int rtc_append(const struct ipt_entry *e, 
		struct rtc_handle *handle);

#endif/*_LIBRT4C*/
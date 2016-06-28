#ifndef _LIBRT4C_H
#define _LIBRT4C_H

#include <linux/types.h>
#include <libiptc/ipt_kernel_headers.h>
#ifdef __cplusplus
#include <climits>
#else
#include <limits.h> /* INT_MAX in ip_tables.h */
#endif
#include <linux/netfilter_ipv4/ip_tables.h>

/* Take a snapshot of the rules.  Returns NULL on error. */
struct rtc_handle *rtc_init();

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

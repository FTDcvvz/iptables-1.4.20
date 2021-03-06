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

/* Sets the policy */
int rtc_set_policy(const char * chain ,const char * policy, const char * tablename);

/* Append to chain  */
int rtc_append(const struct ipt_entry *e);

#endif/*_LIBRT4C*/

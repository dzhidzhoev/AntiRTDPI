//
//  AntiRTDPI.c
//  AntiRTDPI
//
//  Created by Vladislav Dzhidzhoev on 12.08.17.
//  Copyright © 2017 Education Kids. All rights reserved.
//

#include <sys/systm.h>
#include <mach/mach_types.h>
#include <sys/kpi_mbuf.h>
#include <netinet/kpi_ipfilter.h>
#include <netinet/in.h>

kern_return_t AntiRTDPI_start(kmod_info_t * ki, void *d);
kern_return_t AntiRTDPI_stop(kmod_info_t *ki, void *d);

#define BUNDLEID "Education-Kids.AntiRTDPI"

errno_t filter_input_func(void *cookie, mbuf_t *data, int offset, u_int8_t protocol);
errno_t filter_output_func(void *cookie, mbuf_t *data, ipf_pktopts_t options);
void filter_detach_func(void *cookie);

struct ipf_filter filter_struct = {
    BUNDLEID,
    BUNDLEID,
    filter_input_func,
    filter_output_func,
    filter_detach_func
};
ipfilter_t filter;

kern_return_t AntiRTDPI_start(kmod_info_t * ki, void *d)
{
    errno_t err = ipf_addv4(&filter_struct, &filter);
    if (!err)
    {
        printf("AntiRTDPI has started!\n");
    }
    else
    {
        printf("AntiRTDPI error: can't register socket filter, %d\n", err);
        printf("AntiRTDPI has started with error!\n");
    }
    return KERN_SUCCESS;
}

kern_return_t AntiRTDPI_stop(kmod_info_t *ki, void *d)
{
    ipf_remove(filter);
    printf("AntiRTDPI has stopped!\n");
    return KERN_SUCCESS;
}

char get_hex_digit(int digit) {
    if (digit < 10) {
        return (char)digit + '0';
    } else {
        return (char)digit + 'A' - 10;
    }
}

char isgraph (unsigned char c)
{
    if ( c >= 0x21 && c <= 0x7e )
        return 1;
    return 0;
}

void print_mem(char *data) {
    char buf[41];
    for (int i = 0; i < 20; i++) {
        if (isgraph(data[i])) {
            buf[i << 1] = ' ';
            buf[(i << 1) | 1] = data[i];
        } else {
            int x = (unsigned char)data[i];
            int hi = x / 16;
            int lo = x % 16;
            buf[i << 1] = get_hex_digit(hi);
            buf[(i << 1) | 1] = get_hex_digit(lo);
        }
    }
    buf[40] = 0;
    printf("AntiRTDPI %s\n", buf);
}

errno_t filter_input_func(void *cookie, mbuf_t *data, int offset,
                          u_int8_t protocol)
{
    long len = mbuf_len(*data);
    if (len - offset >= 100 && protocol == IPPROTO_TCP)
    {
        unsigned char tcp[100];
        mbuf_copydata(*data, offset, 100, tcp);
        int http_offset = 4 * (tcp[12] >> 4);
        if (100 - http_offset >= 66) {
            unsigned char *buf = &(tcp[http_offset]);
            if (buf[0] == 'H' && buf[1] == 'T')
            {
                if (buf[2] == 'T' && buf[3] == 'P')
                {
                    if (buf[9] == '3' && buf[10] == '0' && buf[11] == '2')
                    {
                        if (buf[56] == 'w' && buf[57] == 'a' && buf[64] == 'r'
                            && buf[65] == 't') {
                            printf("AntiRTDPI triggered!\n");
                            return EAGAIN;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}

errno_t filter_output_func(void *cookie, mbuf_t *data, ipf_pktopts_t options) {
    return 0;
}

void filter_detach_func(void *cookie) {
}

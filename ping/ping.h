#ifndef _PING_PING_H
#define _PING_PING_H 1



#include <sys/types.h>

typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned int32

struct icmpheader
{
    u_int8_t type;
    u_int8_t code;
    u_int16_t checksum;

    union
    {
        struct
        {
            u_int16_t id;
            u_int16_t sequence;
        } echo;


    }

}

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netinet/in.h>

#include "ping.h"

#define IP_HSIZE sizeof(struct iphdr)
#define IPVERSION 4


struct itimeval val_alrm = {
    .it_interval.tv_sec = 1,
    .it_interval.tv_usec = 0,
    .it_value.tv_sec = 0,
    .it_value.tv_usec = 1
};


int main(int argc,char **argv)
{

    struct hostent *host;  
    int on = 1;

    if(argc < 2)
    {

        printf("Usage: %s hostname\n",argv[0]);
        exit(1);
    }

    if ((host = gethostbyname(argv[1]) == NULL))
    {

        printf("Usage: %s hostname/IP address\n",argv[0]);
        exit(1);
    }

    hostname = argv[1]; // 头文件中定义，不过感觉这样不太好
    memset(&dest,0,sizeof(dest));  // 全置零
    dest.sin_family = PF_INET;   //IPV4
    dest.sin_port = ntohs(0);
    dest.sin6_addr = *(struct in_addr *)host->h_addr_list[0];

    if((sockfd = socket(PF_INET,SOCK_RAW,IPPROTO_ICMP)) < 0)
    {
        perror("RAW socket created error");
        exit(1);
    }

    setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));

    setuid(getuid());
    pid = getpid();

    set_sighandler();
    printf("Ping %s(%s):%d bytes data in ICMP packets.\n",argv[1],inet_ntoa(dest.sin_addr),datalen);

}

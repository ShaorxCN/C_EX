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

#define IP_HSIZE sizeof(struct ipheader)
#define IPVERSION 4


struct itimerval val_alrm = {
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

    if ((host = gethostbyname(argv[1])) == NULL)
    {

        printf("Usage: %s hostname/IP address\n",argv[0]);
        exit(1);
    }

    hostname = argv[1]; // 头文件中定义，不过感觉这样不太好
    memset(&dest,0,sizeof(dest));  // 全置零
    dest.sin_family = PF_INET;   //IPV4
    dest.sin_port = ntohs(0);
    dest.sin_addr = *(struct in_addr *)host->h_addr_list[0];

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

    if((setitimer(ITIMER_REAL,&val_alrm,NULL)) == -1)
    {
        bail("setitimer fails.");
    }

    recv_reply(); 

    return 0;

}


void send_ping(void)
{

    struct ipheader *ip_hdr;
    struct icmpheader *icmp_hdr;

    int len;
    int len1;


    ip_hdr = (struct ipheader *)sendbuf;
    ip_hdr->hlen = sizeof(struct ipheader) >> 2;  //参考IHL定义  单位是4字节  所以除以4
    ip_hdr->ver = IPVERSION;
    ip_hdr->tos = 0;  //参考定义 普通
    ip_hdr->tot_len =  IP_HSIZE+ICMP_HSIZE+datalen; // 这是总长度  ip头+数据部分(即icmpt头+icmpdata部分)
    ip_hdr->id = 0;
    ip_hdr->frag_off = 0;    // 无分段 无偏移
    ip_hdr->protocol = IPPROTO_ICMP;
    ip_hdr->ttl = 255;
    ip_hdr->daddr = dest.sin_addr.s_addr;
    len1 = ip_hdr->hlen<<2;

    icmp_hdr = (struct icmpheader *)(sendbuf+len1); //见笔记  icmp实际是ip的data部分
    icmp_hdr->type = ICMP_ECHO; // 见笔记  实际写8也成
    icmp_hdr->code = 0;         // 同见笔记
    icmp_hdr->icmp_id=pid;
    icmp_hdr->icmp_seq=nsent++;
    memset(icmp_hdr->data,0xff,datalen);  //初始化数据段 

    gettimeofday((struct timeval *)icmp_hdr->data,NULL);  // 吧发送送时间作为数据报文 接收计算

    len=ip_hdr->tot_len;
    icmp_hdr->checksum=0;  //先初始化
    icmp_hdr->checksum=checksum((u_int8_t *)icmp_hdr,len);

    sendto(sockfd,sendbuf,len,0,(struct sockaddr *)&dest,sizeof(dest)); //见socket笔记
}


void recv_reply()
{
    int n;
    int len;
    //int errno;

    n = 0;
    nrecv = 0;
    len = sizeof(from);

    // 整四次
    while(nrecv <4)
    {
        if((n = recvfrom(sockfd,recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&from,&len))<0)
        {
            if(errno==EINTR)  // 信号中断
                continue;
            bail("recvfrom error");
        }

        gettimeofday(&recvtime,NULL);  // 记录接收时间

        if(handle_pkt())  //接收错误的icmp
            continue;

            nrecv++;

    }

    get_statistics(nsent,nrecv);   //分析统计
}


// 大概逻辑：首先强制16位一取累加 就2位做1位，然后高于16位的部分拎出来和低16位相加

u_int16_t checksum(u_int8_t *buf,int len)
{
    u_int32_t sum  = 0;
    u_int16_t *cbuf;
    cbuf = (u_int16_t *)buf;

    while(len > 1)
    {
        sum += *cbuf++;
        len -= 2;
    }



    // 如果是奇数 在加上
    if(len)
    {
        sum += *(u_int8_t *)cbuf;
    }

    // 高16位 和低16相加  右移16就是高于16位移到了低16位置前面补0  &0xffff 就是屏蔽了高16位考虑是0000ffff
    // 这就是高低16位相加
    sum = (sum>>16) + (sum&0xffff);
    // 如果这样还是有高于16位的 就直接再加上且是加到低位的  实际已经没有区别了吧
    sum += (sum >> 16);

    // 取反
    return ~sum;
}

// 应答消息处理
int handle_pkt()
{
    struct ipheader *ip;
    struct icmpheader *icmp;
    int ip_hlen;
    u_int16_t ip_datalen;
    double rtt;
    struct timeval  *sendtime;

    ip = (struct ipheader *)recvbuf;

    ip_hlen = ip->hlen << 2;

    ip_datalen = ntohs(ip->tot_len) - ip_hlen;

    icmp = (struct icmpheader *)(recvbuf + ip_hlen);  // 截取icmp部分

    if(checksum((u_int8_t *)icmp,ip_datalen))
        return -1;
    if (icmp->icmp_id != pid)
        return -1;

    sendtime = (struct timeval *)icmp->data;

    rtt = ((&recvtime)->tv_sec - sendtime->tv_sec) *1000 +((&recvtime)->tv_usec - sendtime->tv_usec)/1000.0;  //往返时间

  /*打印结果*/  
	printf("%d bytes from %s:icmp_seq=%u ttl=%d rtt=%.3f ms\n",  \
			ip_datalen,					/*IP数据长度*/  
			inet_ntoa(from.sin_addr),   /*目的ip地址*/  
			icmp->icmp_seq,				/*icmp报文序列号*/  
			ip->ttl,					/*生存时间*/  
			rtt);						/*往返时间*/  
 
	return 0;  
}

void set_sighandler()
{

    act_alarm.sa_handler = alarm_handler;
    if(sigaction(SIGALRM,&act_alarm,NULL) == -1)
    {
        bail("SIGALRM handler setting fails.");
    }

    act_int.sa_handler = int_handler;
    if(sigaction(SIGINT,&act_int,NULL) == -1)
    {
        bail("SIGALRM handler setting fails.");
    }

}


void get_statistics(int nsent,int nrecv)
{
    printf("--- %s ping statistics ---\n",inet_ntoa(dest.sin_addr)); /*将网络地址转换成“.”点隔的字符串格式。*/  
    printf("%d packets transmitted, %d received, %0.0f%% ""packet loss\n",  \
		nsent,nrecv,1.0*(nsent-nrecv)/nsent*100);  
}


void bail(const char * on_what)
{
    fputs(strerror(errno),stderr);
    fputs(":",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);  

}


 /*SIGINT（中断信号）处理程序*/  
void int_handler(int sig)  
{  
    get_statistics(nsent,nrecv);    /*统计ping命令的检测结果*/  
    close(sockfd);  /*关闭网络套接字*/  
    exit(1);  
}  
 
 /*SIGALRM（终止进程）处理程序*/  
 /*settimer */
void alarm_handler(int signo)  
{  
    send_ping();    /*发送ping消息*/  
  
}

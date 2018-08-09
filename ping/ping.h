#ifndef _PING_PING_H
#define _PING_PING_H 1

#define ICMP_ECHOREPLY 0
#define ICMP_ECHO 8

struct icmpheader
{
    u_int8_t type;            //  类型
    u_int8_t code;            //  code
    u_int16_t checksum;       // 校验和

    union
    {
        struct
        {
            u_int16_t id;              // 标识符
            u_int16_t sequence;        // 序列号
        } echo;

        u_int32_t gateway;            //

        struct
        {
            u_int16_t __glibc_reserved;
            u_int16_t mtu;
        } frag;

    } un;

    u_int8_t data[0];

    #define icmp_id un.echo.id
    #define icmp_seq  un.echo.sequence

};

#define ICMP_HSIZE sizeof(struct icmpheader)
#define BUFSIZE 1500 // 发送缓存最大值
#define DEFAULT_LEN 56 // ping 数据默认大小

// ip信息头结构体
struct ipheader
{
    u_int8_t hlen:4,ver:4;  // 首部长度  IP版本号为IPV4
    u_int8_t tos;           // 8位服务类型tos  type of service
    u_int16_t tot_len;      // 总长度 
    u_int16_t id;           // 标识位
    u_int16_t frag_off;     // 标记位+偏移量
    u_int8_t    ttl;        // 生存周期
    u_int8_t    protocol;   // 协议
    u_int16_t   checksum;   // 头校验和
    u_int32_t   saddr;      // 源地址
    u_int32_t   daddr;      // 目标地址

};


char *hostname;   // 存储ping的目标地址
int datalen = DEFAULT_LEN;  // ICMP 消息携带的数据长度
char sendbuf[BUFSIZE];     // 发送的字符串数组
char recvbuf[BUFSIZE];

int nsent;          // 发送的icmp序号 因为icmp其实是在ip消息的数据段  静态去默认0
int nrecv;          // 接收的icmp消息序号

pid_t pid;          // ping进程的pid
struct timeval recvtime; // 接收到icmp的时间
int sockfd;  
struct sockaddr_in dest;
struct sockaddr_in from;
struct  sigaction act_alarm;
struct  sigaction act_int;

void  alarm_handler(int);     // sigalrm 处理函数 sigalrm 单次定时处理
void int_handler(int);        // sigint 处理程序 程序终止信号
void set_sighandler();        // 设置信号处理程序
void send_ping();             // 发送ping
void recv_reply();            
u_int16_t checksum(u_int8_t *buf,int len);  // 第一个其实就是char
int handle_pkt();   // icmp消息处理
void get_statistics(int,int);  // 统计ping命令的检测结果
void bail(const char *);  //错误报告

#endif

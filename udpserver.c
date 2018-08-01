#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define PORT 3490
#define MAXDATASIZE 100

void main()
{
    int sockfd;
    char buf[MAXDATASIZE];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int addr_len,numbytes;


    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero),8);

    if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

    if((numbytes = recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr *)&client_addr,&addr_len)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }

    printf("recvfrom &s\n",inet_ntoa(client_addr.sin_addr));
    printf("data is %d bytes long\n",numbytes);
    buf[numbytes] = '\0';
    printf("package contians \"%s\" \n",buf);
    close(sockfd);

}
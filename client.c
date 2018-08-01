#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <netdb.h>

#define PORT 3490
#define MAXDATASIZE 100

int main(int argc,char *argv[])
{
    int sockfd,numbytes;
    char buf[MAXDATASIZE];

    struct hostent *server;
    struct sockaddr_in server_addr;

    if(argc != 2)
    {
        fprintf(stderr,"usage:client hostname\n");
        exit(1);

    }

    if ((server = gethostbyname(argv[1])) == NULL)
    {
        herror("gethostname");
        exit(1);
    }

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr*)server->h_addr);

    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }

    if((numbytes = recv(sockfd,buf,MAXDATASIZE,0)) == -1)
    {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("recieve from server :%s",buf);
    close(sockfd);
    return 0;


}
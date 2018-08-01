#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#define PORT 3490

int main(int argc,char *argv[])
{
    int sockfd;
    int numbytes;

    struct sockaddr_in server_addr;
    struct hostent *server;

    if(argc != 3)
    {
        fprintf(stderr,"usage : client hostname message\n");
        exit(1);

    }

    if((server = gethostbyname(argv[1])) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }


    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(server_addr.sin_zero),8);


    if((numbytes = sendto(sockfd,argv[2],strlen(argv[2]),0,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))) == -1)
    {
        perror("sendto");
        exit(1);
    }


    printf("send %d bytes to %s\n",numbytes,inet_ntoa(server_addr.sin_addr));
    close(sockfd);
    return 0;

}

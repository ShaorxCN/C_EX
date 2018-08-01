#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/select.h>

#define STDIN 0

void main()
{
    struct timeval tv;
    fd_set readfds;
    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    FD_ZERO(&readfds);
    FD_SET(STDIN,&readfds);

    int recv = select(STDIN+1,&readfds,NULL,NULL,&tv);

    //if(FD_ISSET(STDIN,&readfds))
    if(recv)
        printf("A key was pressed!\n");
    else
        printf("timeout %d\n",FD_ISSET(STDIN,&readfds));


}
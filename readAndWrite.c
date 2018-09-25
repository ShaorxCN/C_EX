#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>


int write_buffer(int fd,const void *buf,size_t count);

int main(int argc,char **argv)
{

    int fd_read,fd_write;
    int read_bytes,write_bytes;
    char buf[128];

    if(argc < 3){
        printf("Usage:cp FILEREAD FILEWRITE\n");
        return -1;
    }

    fd_read = open(argv[1],O_RDONLY);
    if(fd_read == -1){
        perror("open FILEREAD error");
        return -2;
    }

    fd_write = open(argv[2],O_CREAT|O_WRONLY|O_TRUNC,0640);
    if (fd_write == -1){
        perror("open FILEWRITE error");
        return -3;
    }

    while(read_bytes = read(fd_read,buf,sizeof(buf))){
        if(read_bytes > 0){
            write_bytes = write_buffer(fd_write,buf,read_bytes);
            if (write_bytes == -1) {
                perror("write error");
                break;
            }
        }else if((read_bytes == -1) && ( errno != EINTR)){
            /*read error and is  not be interepted*/
            perror("read error");
            break;
        }
    }

    close(fd_read);
    close(fd_write);

    return 0;
}


// write_buffer:ensure the write action
int write_buffer(int fd_write,const void *buf,size_t count)
{
    int n=0,r = count;
    while(count > 0){
        n = write(fd_write,buf,count);
        if (n == 0) break; // end
        if(n > 0){
            count -=n;
            buf+=n;
        }else if(n < 0){
            // 中断的话继续写等
            if(errno == EINTR){
                continue;
            }

            r = -1;
            break;
        }
    }

    return r;
}



#include <unistd.h>
#include <stdio.h>
int main(int argc,char **argv)
{

    int pid;
    int i;
    int status;
    printf("start\n");
    if((pid = fork()) < 0){
        perror("fork error");
        return -1;
    }else if(pid == 0){
        printf("this is child p\n");
        for(i = 0;i<10;i++){
            printf("message from child[%d]\n",i);
            sleep(2);
        }
    }else{
        printf("this is parent p  child p is %d\n",pid);

         for(i = 0;i<5;i++){
            printf("message from parent[%d]\n",i);
            if(i == 3){
                printf("wait block\n");
                pid = wait(&status);  // block.return -1 when no child or other error
            }
            sleep(2);
        }

       

        printf("wait get[%d] status %d\n",pid,status);
    }

    return 0;

}
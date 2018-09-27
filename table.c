#include <stdio.h>

int main(int argc,char **argv)
{
    FILE *fp;
    int i;
    int err;
    char buf[9][10] = {"Company","Address","Project","Zeffi","Shanghai","DC","Evan","Beijing","MV"};

    fp = fopen("table.dat","w");
    if (fp == NULL){
        perror("open error");
        return -1;
    }

    for(i=0;i<9;i+=3){
        fprintf(fp,"%10s%10s%10s\n",buf[i],buf[i+1],buf[i+2]);
        err = ferror(fp);
        if(err != 0){
            perror("write error");
            return -2;
        }
    }

    err = fseek(fp,30,SEEK_SET);
        if(err == -1){
        perror("seek error");
        return -3;
    }

    printf("current file position[%ld]\n",ftell(fp));

    for(i = 0;i<6;i+=3){
        fscanf(fp,"%10s%10s%10s",buf[i],buf[i+1],buf[i+2]);
        printf("%10s%10s%10s\n",buf[i],buf[i+1],buf[i+2]);
    }

    fclose(fp);
    return 0;

}
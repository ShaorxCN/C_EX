#include <stdio.h>
#define SIZE 3


typedef  struct{
    char name[10];
    int num;
}student;

student std[SIZE];
student stdRead[SIZE];

int main(int argc,char **argv)
{
    FILE *fp;
    float fa[10];

    int i;



    if(argc < 2){
        printf("Usage: fwrite FIELNAME\n");
        return -1;
    }   


    fp = fopen(argv[1],"w+");
    if(fp == NULL){
        perror("open file test error");
        return -2;
    }

    for(i = 0;i<SIZE;i++){
        scanf("%s%d\n",std[i].name,&std[i].num);
    }

    i = fwrite(std,sizeof(student),SIZE,fp);

    if(i != SIZE){
        perror("write error");
        fclose(fp);
        return -3;
    }

    i = fseek(fp,0,SEEK_SET);
    if(i != 0){
        perror("seek error");
        return -4;
    }

    i = fread(stdRead,sizeof(std),1,fp);
    if (i != 1){
        perror("read error");
        fclose(fp);
        return -5;
    }
    
    for(i = 0;i<SIZE;i++){
        printf("Name:%s,Num:%d\n",stdRead[i].name,stdRead[i].num);
    }

    fclose(fp);
    return 0;
}


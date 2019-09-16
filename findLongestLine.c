#include <stdio.h>

#define MAXLINE 1000

int getline(char line[],int maxline);

void copy(char to[],char from[]);

int main(){
    int len;
    int max;

    char line[MAXLINE];
    char  longest[MAXLINE];

    max = 0;

    while((len = getline(line,MAXLINE)) >0){
        if(len > max){
            max = len;
            copy(longest,line);
        }
    }

    if(max > 0)
        printf("%s\n",longest);
    

    return 0;
}


int getline(char a[],int limit){
    int c,i;

    for(i=0;i<limit;++i){
        
    }
}
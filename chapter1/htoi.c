#include <stdio.h>

#define YES 1
#define NO 0
int htoi(char s[]);

int htoi(char s[])
{
    int c,i,hexdigit,inhex,n;

    i=0;
    
    if(s[i] == '0'){
        ++i;
        if(s[i]=='x' || s[i] == 'X')
            ++i;
        else
            return -1;
        
    }else{
        return -1;
    }

    inhex=YES;
    n=0;

    for(;inhex == YES;++i){
        printf("%c\n",s[i]);
        if(s[i] >= '0' && s[i]<='9')
            hexdigit = s[i]-'0';
        else if (s[i] >= 'a' && s[i] <= 'f')
            hexdigit = s[i]-'a'+10;
        else if(s[i] >= 'A' && s[i] <= 'F')
            hexdigit = s[i]-'A'+10;
        else
            inhex = NO;
        
        if(inhex == YES)
            n = 16*n+hexdigit;
    }

    return n;
}

int main()
{
    char input[] = "0x123";

    printf("%d\n",htoi(input));
}
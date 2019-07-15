#include <stdio.h>

int main()
{
    double nc;
    int c;

    int lineCount;
    for(nc = 0;(c = getchar()) != EOF ;++nc){
        if (c == '\n'){
            --nc;
            ++lineCount;
        }
    }
    printf("%.0f %d\n",nc,lineCount);

    return 0;
}
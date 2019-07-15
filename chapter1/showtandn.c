#include <stdio.h>

int main(){
    int c;
    while((c = getchar()) != EOF){
        if (c == '\t')
            printf("\\t");
        else if (c == '\n')
            printf("\\n");
        else
            putchar(c);
    }

    return 0;
}
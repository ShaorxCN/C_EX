#include <stdio.h>

int main(){
    float fahr,celsius;
    int lower,upper,step;

    lower = 0;
    upper = 300;
    step = 20;

    fahr = lower;
     printf("centi - > fahr\n");
    while(fahr <= upper){
        // tip: 5/9 = 0 so 5 * x / 9
        celsius = 5.0*(fahr-32.0)/9.0;
        printf("%3.0f\t%6.1f\n",fahr,celsius);
        fahr = fahr+step;
    }
}
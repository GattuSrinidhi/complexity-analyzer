#include <stdio.h>

int main(){
    int n = 10;

    for(int i=0;i<n;i++){

        for(int j=0;j<n;j++){

            if(i==j && j>0){
                printf("Hello");
            }

        }

    }

    return 0;
}
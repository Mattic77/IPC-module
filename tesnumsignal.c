#include<signal.h>
#include<stdio.h>
#include<string.h>

int main(){
    for(int signum=0;signum<_NSIG;signum++){
        fprintf(stderr,"%2d : %s \n",signum,strsignal(signum));
    }
    return 0;
}
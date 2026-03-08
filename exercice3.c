#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include <sys/types.h>

#define TAILLE 10
__pid_t tabpid[TAILLE];
void sigcount(int numsignal){
    printf("signal sigcont recu  fils =%d \n",getpid());
    fflush(stdout);
}

int main(){


    

    for (size_t i = 0; i < TAILLE; i++)
    {
         tabpid[i]=fork();
        if(tabpid[i]<0){
            perror("error dans la creation sur fils");
        }
        if(tabpid[i]==0){
            signal(SIGCONT,sigcount);
            while(1){
            pause();
        }
        }
    }
    int courant=0;
    kill(tabpid[courant],SIGCONT);
    while(1){
        if(courant<=TAILLE){
        sleep(1);
        kill(tabpid[courant],SIGSTOP);
        courant++;
        kill(tabpid[courant],SIGCONT);
       }else {
        exit(0);
       }

    }
    
    
       
    }

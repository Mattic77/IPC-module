#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<string.h>
int impairsval=1;
int pairsval=0;
__pid_t pid_fils=-1;
void impairs(int numsignal){
    if(impairsval<=99){
        printf("fils= %d \n",impairsval);
        fflush(stdout);
        impairsval+=2;
        kill(getppid(),SIGUSR1);
    }else{
        exit(0);
    }
    
}
void pairs(int numsignal){
       if(pairsval<=100){
        printf("pere = %d \n",pairsval);
        fflush(stdout);
        pairsval+=2;
    }else{
        exit(0);
    }
    if(pid_fils>0){
        kill(pid_fils,SIGUSR2);
    }
    
    
}


int main(){

 __pid_t pid;

 pid = fork();
 if(pid<0){
    perror("error dans la creation du fork");
 }
 if (pid==0)
 {
    signal(SIGUSR2,impairs);
    while (1)
    {
    pause();
    }
    
 }else{
    pid_fils=pid;
    signal(SIGUSR1,pairs);
    kill(getpid(),SIGUSR1);
   while (1)
    {
    pause();
    }
 }
 

}
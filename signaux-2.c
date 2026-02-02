#include <stdio.h>
#include <signal.h>
#include<stdlib.h>
#include<unistd.h>


void trait_signint( int signum)
{
    printf("bien recu SIGINT, mais je vais  lignorer \n");
}

void trait_sigquit( int signum)
{
    printf("bien recu SIQUIT, mais je vais  lignorer \n");
}

int main()
{

    int pid;
    pid=fork();
    if(pid==0)
    {
        signal(SIGINT,trait_signint);
        signal(SIGQUIT,trait_sigquit);

        printf("je suis toujours en vie \n");
        sleep(20);
        printf("premeir reveil du fils \n");
        sleep(120);
        printf(" second reveil du fils \n");
        sleep(500);
        printf(" troisieme reveil du fils \n");


    }else{
        sleep(1);
        kill(pid,SIGINT);
        sleep(2);
        kill(pid,SIGQUIT);
        sleep(5);
        kill(pid,SIGKILL);
    }
    return 0;

}
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int tour = 0;

void handler(int sig) {
    tour = 1;
}

int main() {
    signal(SIGUSR1, handler);
    
    int pid = fork();
    
    if (pid == 0) {
        
        for (int i = 1; i < 100; i += 2) {
            while (tour == 0) pause(); 
            printf("%d child  + tour %d \n"  , i,tour);
            fflush(stdout);
            tour = 0;
            kill(getppid(), SIGUSR1); 
        }
        exit(0);
    } else {
       
        for (int i = 0; i <= 100; i += 2) {
            printf("%d  parent  + tour %d \n", i,tour);
            fflush(stdout);
            tour = 0;
            kill(pid, SIGUSR1); 
            if (i < 100) {
                while (tour == 0) pause(); 
            }
        }
        printf("\n");
        wait(NULL);
    }
    
    return 0;
}
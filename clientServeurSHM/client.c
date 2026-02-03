#include <sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>

#define SHM 40

int main(){

    char *tab;
    int shmid;
    
    shmid=shmget((size_t)SHM,0,0);
    if(shmid==-1){
        perror("SHMGET");
        
    }
    tab=(char*) shmat(shmid,NULL,0);
    while (1)
    {
        printf("saisir une text");
        scanf("%s",tab);
        
    }
    


}
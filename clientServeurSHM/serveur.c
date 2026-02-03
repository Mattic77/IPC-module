#include <sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>

#define SHM 40
#define VAL 20
int main(){

    int shmid;
    char *tab;
    

    shmid=shmget((key_t)SHM,VAL*sizeof(char),IPC_CREAT|S_IRUSR|S_IWUSR);
    if(shmid==-1){
        perror("SHMGET");
        
    }
    tab= (char*) shmat(shmid,NULL,0);
    
    while(1){
        for (int  i = 0; i < VAL; i++)
        {
            printf("tab[%d] = %s\n", i, tab[i]);
            sleep(1);
        }
        
    }

}
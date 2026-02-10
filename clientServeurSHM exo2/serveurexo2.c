#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SHM 40

char *tab;
int shmid;

void handler(int sig){
   printf("\nSignal reçu, fermeture du serveur...\n");
    struct shmid_ds buf;
    shmctl(shmid,IPC_STAT,&buf);
    
    shmdt(tab);
    if(buf.shm_nattch == 0){
        shmctl(shmid, IPC_RMID, NULL);
        printf("Mémoire partagée détruite\n");
    }
    exit(EXIT_SUCCESS);
}

int main(){
    signal(SIGINT, handler);
    
    shmid = shmget((key_t)SHM, 20, IPC_CREAT | 0666);
    if(shmid == -1){
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }
    
    tab = (char*) shmat(shmid, NULL, 0);
    memset(tab, 0, 20);
    
    printf("Serveur démarré. Affichage du contenu toutes les secondes...\n");
    
    while(1){
        sleep(1);
        printf("Contenu: %s\n", tab + sizeof(int)); 
    }
    
    return 0;
}
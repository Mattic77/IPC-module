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
    printf("\nSignal reçu, fermeture du client...\n");
    shmdt(tab);
    exit(EXIT_SUCCESS);
}

int main(){
    signal(SIGUSR1, handler);
    
    shmid = shmget((key_t)SHM, 20, 0);
    if(shmid == -1){
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }
    
    tab = (char*) shmat(shmid, NULL, 0);
    
    int pid = getpid();
    memcpy(tab, &pid, sizeof(int));
    printf("PID %d envoyé au serveur\n", pid);
    
    while(1){
        printf("Saisir un texte (max 15 caractères): ");
        scanf("%15s", tab + sizeof(int)); 
    }
    
    return 0;
}
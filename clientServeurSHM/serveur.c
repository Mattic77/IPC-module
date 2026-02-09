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
    printf("\n\nFermeture du serveur...\n");
    
    int client_pid;
    memcpy(&client_pid, tab, sizeof(int));
    printf("PID du client récupéré: %d\n", client_pid);
    if(client_pid > 0){
        printf("Envoi du signal au client (PID: %d)\n", client_pid);
        kill(client_pid, SIGUSR1);
        sleep(1);
    }
    
    shmdt(tab);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Mémoire partagée détruite\n");
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
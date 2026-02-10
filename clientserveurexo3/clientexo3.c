#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHM 60

struct str{
    int pid;
    int cmd;
    char text[500];
    char nom[10];
    char prenom[10];
};

int shmid;

int main(){
    struct str data;
    struct str *shm;

    printf("connexion au serveur en cours...\n");
    shmid = shmget((key_t)SHM, sizeof(struct str), IPC_CREAT | 0666);
    if(shmid == -1){
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }

    shm = (struct str*) shmat(shmid, NULL, 0);
    if(shm == (void*)-1){
        perror("SHMAT");
        exit(EXIT_FAILURE);
    }

    data.pid = getpid();
    printf("veuillez entrer votre nom : ");
    scanf("%9s", data.nom);
    printf("veuillez entrer votre prenom : ");
    scanf("%9s", data.prenom);

    while(1){
        printf("choisir une commande : \n1- envoyer un message \n2- deconnecter\n");
        scanf("%d", &data.cmd);

        if(data.cmd == 1){
            printf("veuillez entrer votre message : ");
            scanf("%499s", data.text);
            *shm = data; 
        }else if(data.cmd == 2){
            *shm = data;
            printf("deconnexion en cours...\n");
            shmdt(shm);
            exit(EXIT_SUCCESS);
        }
        sleep(1);
    }
    return 0;
}
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

struct str *data;
struct str tab[100];
int shmid;

int main(){
    int count = 0;

    shmid = shmget((key_t)SHM, sizeof(struct str), IPC_CREAT | 0666);
    if(shmid == -1){
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }

    data = (struct str*) shmat(shmid, NULL, 0);
    if(data == (void*)-1){
        perror("SHMAT");
        exit(EXIT_FAILURE);
    }

    printf("Serveur démarré...\n");
    while(1){
        if(data->cmd == 1){
            printf("message de %s %s : %s\n", data->nom, data->prenom, data->text);
            if(count < 100){
                tab[count++] = *data;
            }
            data->cmd = 0; 
        }else if(data->cmd == 2){
            printf("deconnexion de %s %s (PID: %d)\n", data->nom, data->prenom, data->pid);
            data->cmd = 0;
        usleep(100000);
    }
}
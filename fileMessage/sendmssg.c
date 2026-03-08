#include <stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<string.h>
#define TAILEMSSG 255

struct mssgtxt 
{
    long mtype;
    char mtext[TAILEMSSG];
};
int main(){
    int mssgid;
    key_t cle;
     printf("Entrer la cle associe a la file");
     scanf("%d",&cle); 
     mssgid= msgget(cle,IPC_CREAT|S_IRUSR|S_IWUSR);
     if(mssgid==-1){
        fprintf(stderr,"erreur dans la creation");
        exit(1);

     }
     struct mssgtxt msg;

     msg.mtype=12;
     strcpy(msg.mtext,"bonjour younes");
     int ret=msgsnd(mssgid,&msg,TAILEMSSG,0);
     if(ret== -1){
        perror("ERROR DANS EMISSION");
        exit(2);
     }
     printf("Message bien envoye  type=%d",msg.mtype);
     
  

    return 0;
}
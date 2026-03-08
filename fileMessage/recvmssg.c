#include <stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>

#define TAILEMSSG 255

struct mssgtxt 
{
    long mtype;
    char mtext[TAILEMSSG];
};

int main(){
    key_t cle ;
    int msgid;
    long type;

    struct msqid_ds buf;
    struct mssgtxt msg;
    printf("Entrer la cle associe a la file");
     scanf("%d",&cle); 
       msgid= msgget(cle,0);

       printf("entrer le type de message");
       scanf("%ld",&type);
       if((msgrcv(msgid,&msg,TAILEMSSG,type,IPC_NOWAIT)==-1)){
        fprintf(stderr,"pas de message \n");

       }
        printf("messqge lu %s\n",msg.mtext);
        printf("pid du process courant %d \n",getpid());
        msgctl(msgid,IPC_STAT,&buf);
        printf("nombre de message apres lecture  %d \n",buf.msg_qnum);
         printf("pid du dernier processus lecture  %d\n",buf.msg_lrpid);

    return 0;
}
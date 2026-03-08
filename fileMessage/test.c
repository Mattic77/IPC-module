#include <stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>

int main (){
  key_t cle;
  int msgid;
  struct msqid_ds buf;
 
  
  printf("Entrer la cle associe a la file");
  scanf("%d",&cle);
  msgid= msgget(cle,0);
  if(msgctl(msgid,IPC_STAT,&buf)){
    fprintf(stderr,"erreur dacces aux information");
    exit(1);
  }
  printf("caracteristique %d \n",cle);
  printf("mo;bre de messqge %d \n",msgid);
  printf("pid du dernier process lecture %d \n",buf.msg_lrpid);
  return 0;
  


}
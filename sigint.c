#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

int num_signal=0;
void traiter_SIGINT(int signum){
    num_signal++;
    printf("Signal CTRL-C capture! \n");

}
int main(){
  if(signal(SIGINT,traiter_SIGINT)==SIG_ERR){
    printf("erreur dans le gestionnaire \n");
    exit(1);
  }
  while (num_signal<5){
    pause();
    printf("\n %d signaux SIGINT sont arrives \n",num_signal);
  }
  printf("5 signaux recus, programme termine!\n");
  return 0;
  
  

}
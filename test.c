#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include<unistd.h>
#include<stdlib.h>


   
   void sighandler(int signum);
   int main(){
    char buffer[256];
    
    
    if(signal(SIGTERM,SIG_IGN)==SIG_ERR){
        printf("ne peut pas manupiler le signal \n");
        exit(1);
    }
    while (1)
    {
     fgets(buffer,sizeof(buffer),stdin);
     printf("input %s",buffer);
    }
    return 0;
    

   }
   void sighandler(int signum){
    printf("Masqugue du signal SIFTERM \n");
   }


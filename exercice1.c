#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<string.h>

int tab_signal[_NSIG]={0};

void ajouter_Signal(int num_signal){
    tab_signal[num_signal]++;
    printf("la nouvelle valeur de la cae numero %d est %d",num_signal,tab_signal[num_signal]);
}

void main(){
    for (int  i = 0; i <_NSIG ; i++)
    {
        signal(i,ajouter_Signal);
        printf("BLOCK");
        
    }
    while(1){
        pause();
    }


    
}
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define TAILLE 2

pid_t tabpid[TAILLE];
int mon_index = -1;

void filsfunction(int numsignal) {
    (void)numsignal;

    printf("je suis le fils n=%d avec le pid=%d\n", mon_index, getpid());
    fflush(stdout);

    if (mon_index == TAILLE - 1) {
        /* dernier fils -> reveille le pere */
        kill(getppid(), SIGCONT);
    } else {
        /* reveille le fils suivant */
        kill(tabpid[mon_index + 1], SIGCONT);
    }
}

void pere(int numsignal) {
    (void)numsignal;
}

int main(void) {
    signal(SIGCONT, pere);

    /* creation en ordre inverse pour que chaque fils connaisse le suivant */
    for (int i = TAILLE - 1; i >= 0; i--) {
        tabpid[i] = fork();

        if (tabpid[i] < 0) {
            perror("fork");
            exit(1);
        }

        if (tabpid[i] == 0) {
            mon_index = i;
            signal(SIGCONT, filsfunction);

            while (1) {
                pause();
            }
        }
    }

    sleep(1); /* laisse le temps aux fils d'installer leur handler */

    while (1) {
        printf("nouveau cycle\n");
        fflush(stdout);

        kill(tabpid[0], SIGCONT); /* demarre la chaine */
        pause();                  /* reveille par le dernier fils */
    }
}
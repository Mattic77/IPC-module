#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <map>
#include <string>

#define TAILLE 255
#define CLE_SERVEUR 1234

// Actions possibles
#define CONNECT 1
#define LIST 2
#define SEND_PRIVATE 3
#define BROADCAST 4
#define DISCONNECT 5

struct Message {
    long mtype;          // Type pour le routage
    int action;          // Action à effectuer
    int sender_pid;      // PID expéditeur
    char pseudo[50];     // Pseudo du client
    int dest_pid;        // PID destinataire
    char texte[TAILLE];  // Contenu du message
};

int main(){
    key_t cle_serveur = CLE_SERVEUR;
    key_t cle_client = getpid();  // Clé unique = PID
    int msgid_serveur;
    int msgid_client;
    struct Message msg;
    char pseudo[50];
    int choix;
    
    // 1. CRÉER LA FILE CLIENT
    msgid_client = msgget(cle_client, IPC_CREAT|S_IRUSR|S_IWUSR);
    if(msgid_client == -1){
        perror("Erreur création file client");
        exit(1);
    }
    printf("Client démarré. PID: %d, File ID: %d\n", getpid(), msgid_client);
    
    // 2. OUVRIR LA FILE SERVEUR (sans créer)
    msgid_serveur = msgget(cle_serveur, 0);
    if(msgid_serveur == -1){
        perror("Erreur: serveur non trouvé");
        msgctl(msgid_client, IPC_RMID, NULL);
        exit(1);
    }
    
    // 3. CONNEXION AU SERVEUR
    printf("Entrez votre pseudo: ");
    scanf("%s", pseudo);
    
    Message demande;
    demande.mtype = 1;  // Type = 1 pour le serveur
    demande.action = CONNECT;
    demande.sender_pid = getpid();
    strcpy(demande.pseudo, pseudo);
    
    if(msgsnd(msgid_serveur, &demande, sizeof(demande)-sizeof(long), 0) == -1){
        perror("Erreur envoi connexion");
        msgctl(msgid_client, IPC_RMID, NULL);
        exit(1);
    }
    printf("Connexion en cours...\n");
    
    // Attendre confirmation
    if(msgrcv(msgid_client, &msg, sizeof(msg)-sizeof(long), getpid(), 0) != -1){
        printf("Connecté! Message: %s\n", msg.texte);
    }
    
    // 4. MENU PRINCIPAL
    while(1){
        printf("\n========== MENU ==========\n");
        printf("1. Lister les clients connectés\n");
        printf("2. Envoyer message privé\n");
        printf("3. Envoyer message à tous\n");
        printf("4. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);
        getchar();  // Vider le buffer
        
        switch(choix){
            case 1: {
                // DEMANDER LA LISTE
                Message req;
                req.mtype = 1;
                req.action = LIST;
                req.sender_pid = getpid();
                
                if(msgsnd(msgid_serveur, &req, sizeof(req)-sizeof(long), 0) == -1){
                    perror("Erreur envoi liste");
                } else {
                    printf("\nAttente de la liste...\n");
                    if(msgrcv(msgid_client, &msg, sizeof(msg)-sizeof(long), getpid(), 0) != -1){
                        printf("Clients connectés:\n%s\n", msg.texte);
                    }
                }
                break;
            }
            
            case 2: {
                // MESSAGE PRIVÉ
                char dest_pseudo[50];
                char message[TAILLE];
                int dest_pid;
                
                printf("Pseudo destinataire: ");
                scanf("%s", dest_pseudo);
                getchar();
                
                printf("Message: ");
                fgets(message, TAILLE-1, stdin);
                message[strlen(message)-1] = '\0';  // Enlever le \n
                
                printf("PID destinataire: ");
                scanf("%d", &dest_pid);
                getchar();
                
                Message prive;
                prive.mtype = 1;
                prive.action = SEND_PRIVATE;
                prive.sender_pid = getpid();
                prive.dest_pid = dest_pid;
                strcpy(prive.pseudo, pseudo);
                strcpy(prive.texte, message);
                
                if(msgsnd(msgid_serveur, &prive, sizeof(prive)-sizeof(long), 0) == -1){
                    perror("Erreur envoi message privé");
                } else {
                    printf("Message envoyé!\n");
                }
                break;
            }
            
            case 3: {
                // BROADCAST
                char message[TAILLE];
                
                printf("Message pour tous: ");
                fgets(message, TAILLE-1, stdin);
                message[strlen(message)-1] = '\0';  // Enlever le \n
                
                Message broadcast;
                broadcast.mtype = 1;
                broadcast.action = BROADCAST;
                broadcast.sender_pid = getpid();
                strcpy(broadcast.pseudo, pseudo);
                strcpy(broadcast.texte, message);
                
                if(msgsnd(msgid_serveur, &broadcast, sizeof(broadcast)-sizeof(long), 0) == -1){
                    perror("Erreur envoi broadcast");
                } else {
                    printf("Message envoyé à tous!\n");
                }
                break;
            }
            
            case 4: {
                // DÉCONNEXION
                Message deconnexion;
                deconnexion.mtype = 1;
                deconnexion.action = DISCONNECT;
                deconnexion.sender_pid = getpid();
                strcpy(deconnexion.pseudo, pseudo);
                
                if(msgsnd(msgid_serveur, &deconnexion, sizeof(deconnexion)-sizeof(long), 0) == -1){
                    perror("Erreur déconnexion");
                }
                
                printf("Déconnexion...\n");
                msgctl(msgid_client, IPC_RMID, NULL);
                return 0;
            }
            
            default:
                printf("Choix invalide!\n");
        }
        
        // Vérifier s'il y a des messages reçus (non-bloquant)
        if(msgrcv(msgid_client, &msg, sizeof(msg)-sizeof(long), getpid(), IPC_NOWAIT) != -1){
            if(msg.action == SEND_PRIVATE){
                printf("\n[Message privé de %s (PID: %d)]: %s\n", 
                       msg.pseudo, msg.sender_pid, msg.texte);
            } else if(msg.action == BROADCAST){
                printf("\n[Message de %s (PID: %d)]: %s\n", 
                       msg.pseudo, msg.sender_pid, msg.texte);
            }
        }
    }
    
    return 0;
}
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
#define CLE_SERVEUR 1234  // Clé fixe connue par tous

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
    int dest_pid;        // PID destinataire (pour SEND_PRIVATE)
    char texte[TAILLE];  // Contenu du message
};

int main(){
    key_t cle = CLE_SERVEUR;
    std::map<int, std::string> clients;     
    int msgid_serveur;
    struct Message msg;
    
    // 1. CRÉER LA FILE SERVEUR
    msgid_serveur = msgget(cle, IPC_CREAT|S_IRUSR|S_IWUSR);
    if(msgid_serveur == -1){
        perror("Erreur création file serveur");
        exit(1);
    }
    printf("Serveur démarré. File ID: %d\n", msgid_serveur);
    
    // 2. BOUCLE PRINCIPALE
    while(1){
        // Attendre un message (type=0 = n'importe quel type)
        if(msgrcv(msgid_serveur, &msg, sizeof(msg)-sizeof(long), 0, 0) == -1){
            perror("Erreur réception");
            continue;
        }
        
        printf("Message reçu de PID %d, action: %d\n", msg.sender_pid, msg.action);
        
        // 3. TRAITER SELON L'ACTION
        switch(msg.action){
            case CONNECT: {
                // Ajouter client à la map
                clients[msg.sender_pid] = std::string(msg.pseudo);
                printf("Client connecté: %s (PID: %d)\n", msg.pseudo, msg.sender_pid);
                
                // Envoyer confirmation au client
                int client_msgid = msgget(msg.sender_pid, 0);
                if(client_msgid != -1){
                    Message reponse;
                    reponse.mtype = msg.sender_pid;  // Type = PID client
                    reponse.action = CONNECT;
                    strcpy(reponse.texte, "Connexion réussie");
                    msgsnd(client_msgid, &reponse, sizeof(reponse)-sizeof(long), 0);
                }
                break;
            }
            
            case LIST: {
                // Construire la liste des pseudos
                std::string liste;
                for(auto &c : clients){
                    liste += std::to_string(c.first) + ":" + c.second + "\n";
                }
                
                // Envoyer au client demandeur
                int client_msgid = msgget(msg.sender_pid, 0);
                if(client_msgid != -1){
                    Message reponse;
                    reponse.mtype = msg.sender_pid;
                    reponse.action = LIST;
                    strncpy(reponse.texte, liste.c_str(), TAILLE-1);
                    msgsnd(client_msgid, &reponse, sizeof(reponse)-sizeof(long), 0);
                }
                printf("Liste envoyée à %d\n", msg.sender_pid);
                break;
            }
            
            case SEND_PRIVATE: {
                // Vérifier que le destinataire existe
                if(clients.find(msg.dest_pid) != clients.end()){
                    // Envoyer au destinataire
                    int dest_msgid = msgget(msg.dest_pid, 0);
                    if(dest_msgid != -1){
                        Message transfert;
                        transfert.mtype = msg.dest_pid;
                        transfert.action = SEND_PRIVATE;
                        transfert.sender_pid = msg.sender_pid;
                        strcpy(transfert.pseudo, clients[msg.sender_pid].c_str());
                        strcpy(transfert.texte, msg.texte);
                        msgsnd(dest_msgid, &transfert, sizeof(transfert)-sizeof(long), 0);
                        printf("Message privé relayé de %d vers %d\n", msg.sender_pid, msg.dest_pid);
                    }
                } else {
                    printf("Destinataire %d introuvable\n", msg.dest_pid);
                }
                break;
            }
            
            case BROADCAST: {
                // Envoyer à tous les clients
                for(auto &c : clients){
                    if(c.first != msg.sender_pid){  // Pas à soi-même
                        int client_msgid = msgget(c.first, 0);
                        if(client_msgid != -1){
                            Message diffusion;
                            diffusion.mtype = c.first;
                            diffusion.action = BROADCAST;
                            diffusion.sender_pid = msg.sender_pid;
                            strcpy(diffusion.pseudo, clients[msg.sender_pid].c_str());
                            strcpy(diffusion.texte, msg.texte);
                            msgsnd(client_msgid, &diffusion, sizeof(diffusion)-sizeof(long), 0);
                        }
                    }
                }
                printf("Broadcast de %d à tous\n", msg.sender_pid);
                break;
            }
            
            case DISCONNECT: {
                // Retirer de la map
                printf("Client déconnecté: %s (PID: %d)\n", 
                       clients[msg.sender_pid].c_str(), msg.sender_pid);
                clients.erase(msg.sender_pid);
                break;
            }
            
            default:
                printf("Action inconnue: %d\n", msg.action);
        }
    }
    
    // Nettoyage (jamais atteint dans cette version)
    msgctl(msgid_serveur, IPC_RMID, NULL);
    return 0;
}
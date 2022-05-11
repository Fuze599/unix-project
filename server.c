#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utils.h"
#include "utils_v1.h"


volatile sig_atomic_t end = 0;

#define PERM 0666

#define BACKLOG 5

void sigint_handler (int sig) {
  char *msg = "Fin du serveur de virements\n";
  size_t sz = strlen(msg);
  nwrite(0, msg, sz);
  end=1;
}

int initSocketServer(int port) {
  // socket creation
  int sockfd = ssocket();
  sbind(port, sockfd);
  slisten(sockfd, BACKLOG);
  return sockfd;
}

int main(int argc, char **argv) {

  ListVirements listVirement;

  sigset_t set;
  ssigemptyset (&set);
  sigaddset(&set, SIGINT);

  ssigaction(SIGINT, sigint_handler);

  int port = atoi(argv[1]);
	int sockfd = initSocketServer(port);
	printf("Le serveur tourne sur le port : %i \n", port);

  // get shared memory
	int shm_id = sshmget(SHM_KEY, NB_CLIENT * sizeof(int), IPC_CREAT | PERM);
	int* ptns = sshmat(shm_id);

  int sem_id = sem_get(SEM_KEY, 1);

  int sommeMontants=0;
  char messagePourClient [BUFFER_SIZE];
  int nbVirements=0;
  
  while (!end) {
    // make the operation 
    int newsockfd = accept(sockfd, NULL, NULL);
    //if ctrl+c
    if(end){
      break;
    }

    read(newsockfd, &listVirement, sizeof(listVirement));
    //if ctrl+c
    if(end){
      sclose(newsockfd);
      break;
    }

    nbVirements=listVirement.tailleLogique;
    sommeMontants=0;

    sem_down0(sem_id);
    for(int i=0;i<nbVirements;i++){
      int num_emeteur = listVirement.listVirements[i].num_emeteur;
      int num_beneficiaire = listVirement.listVirements[i].num_beneficiaire;
      int montant = listVirement.listVirements[i].montant;
      sommeMontants+=montant;

      int emeteurCompte = *(ptns+num_emeteur);
      int beneficiaireCompte = *(ptns+num_beneficiaire);

	    *(ptns+num_emeteur) = emeteurCompte - montant;
      *(ptns+num_beneficiaire) = beneficiaireCompte + montant;
      sprintf(messagePourClient, "Nouvelle somme : %d\n", *(ptns+num_emeteur));
    }
    
    sem_up0(sem_id);

    //sprintf(messagePourClient,"Il y a eu %d virements pour un montant total de %d euros", nbVirements, sommeMontants);
    
    nwrite(newsockfd, &messagePourClient, strlen(messagePourClient));
    

    sclose(newsockfd);
  }

  sshmdt(ptns);
  sclose(sockfd);
}

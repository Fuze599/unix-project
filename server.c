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
volatile sig_atomic_t canEnd = 1;

#define PERM 0666

#define BACKLOG 5

int initSocketServer(int port)
{
  // socket creation
  int sockfd = ssocket();

  sbind(port, sockfd);


  slisten(sockfd, BACKLOG);


  return sockfd;
}

int main(int argc, char **argv) {

  structListVirement listVirement;

	int sockfd = initSocketServer(atoi(argv[1]));
	printf("Le serveur tourne sur le port : %i \n", sockfd);

  // get shared memory
	int shm_id = sshmget(SHM_SEM_KEY, NB_CLIENT * sizeof(int), IPC_CREAT | PERM);
	int* ptns = sshmat(shm_id);

  int sem_id = sem_get(SHM_SEM_KEY, 1);
  
  while (!end)
  {
    // make the operation 
    int newsockfd = accept(sockfd, NULL, NULL);

	  sem_down0(sem_id);
	
    read(newsockfd, &listVirement, sizeof(listVirement));

    for(int i=0;i<listVirement.tailleLogique;i++){
      int num_emeteur = listVirement.listVirements[i].num_emeteur;
      int num_beneficiaire = listVirement.listVirements[i].num_beneficiaire;
      int montant = listVirement.listVirements[i].montant;


      int emeteurCompte = *(ptns+num_emeteur);
      int beneficiaireCompte = *(ptns+num_beneficiaire);

	    *(ptns+num_emeteur) = emeteurCompte - montant;
      *(ptns+num_beneficiaire) = beneficiaireCompte + montant;
    }

    sem_up0(sem_id);
  }

  sshmdt(ptns);
  sclose(sockfd);
}

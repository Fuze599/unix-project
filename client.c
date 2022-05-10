#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "utils_v1.h"



void virementsReccurents(void* pipefd, void* address, void* port){
	Virement all_virements[100]; 

	int indice = 0;
	int* ptnPipeFd = pipefd;
	int* ptnPort = (int*) port;
	Virement virementRecu;
	char buffer[256];
	sclose(ptnPipeFd[1]);
	while(sread(ptnPipeFd[0], &virementRecu, sizeof(Virement))){
		if(virementRecu.num_emeteur == -1){
			printf("il faut exécuter les virements réccurents ici\n");
			//Liste virements
			ListVirements listvirementStruct;
  		listvirementStruct.tailleLogique = indice;
  		memcpy(listvirementStruct.listVirements, &all_virements, sizeof(all_virements));
  		// Création socket pour interagir avec server
			int sockfd = ssocket();
  		sconnect(address, *ptnPort, sockfd);
  		swrite(sockfd, &listvirementStruct, sizeof(listvirementStruct));
  		sread(sockfd, &buffer, sizeof(buffer));
  		printf("%s\n", buffer);
  		sclose(sockfd);
		}
		else{ // on ajoute à la liste
			all_virements[indice++] = virementRecu;
			printf("On ajoute a la liste %d \n", all_virements[indice - 1].montant);
		}
	}
	sclose(ptnPipeFd[0]);
}

void childTimer(void *delay, void *pipefd) {
	int* ptnPipeFd = pipefd;
	sclose(ptnPipeFd[0]);
	int* ptn = (int*) delay;
	Virement fakeVirement;
	fakeVirement.num_emeteur = -1;
	while(1){
		sleep(*ptn);
		swrite(ptnPipeFd[1], &fakeVirement, sizeof(int));
	}
	sclose(ptnPipeFd[1]);

}

int main(int argc, char **argv) {
  if (argc != 5) {
  	perror("Not enough args");
  	exit(1);
  }

  char* address = argv[1];
  int port = atoi(argv[2]), num = atoi(argv[3]), delay = atoi(argv[4]);
  //create a pipe
  int pipefd[2];
  spipe(pipefd);
  //sclose(pipefd[0]);

  int childTimerId = fork_and_run2(childTimer, &delay, &pipefd);
  int childVirementsReccurents = fork_and_run3(virementsReccurents, &pipefd, &address, &port);
  printf("%d, %d, %d, %d, %d \n", childTimerId, childVirementsReccurents, port, num, delay);
  sclose(pipefd[0]);
  printf("Entrez une commande > ");
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
  	buffer[strlen(buffer) - 1] = '\0';
    const char* separator = " ";
    char* strToken = strtok (buffer, separator);

    if (strToken == NULL) {
    	perror("Bad arguments");
    	printf("Entrez une commande > ");
    	continue;
    }

    strcpy(buffer, strToken);

    if (strcmp(strToken, "+") == 0 || strcmp(strToken, "*") == 0) {
			strToken = strtok (NULL, separator);
			int accountNb = atoi(strToken);
  		strToken = strtok (NULL, separator);
  		int amount = atoi(strToken);
  		if (amount == 0) {
  			printf("Le montant ne peux pas être nul\n");
  			printf("Entrez une commande > ");
  			continue;
  		}
  		int sockfd = ssocket();
  		sconnect(address, port, sockfd);
			Virement virement = {num, accountNb, amount};
  		Virement listVirements[100];
  		
  		if (strcmp(buffer, "+") == 0) {
  			listVirements[0] = virement;
	  		ListVirements listvirementStruct;
	  		listvirementStruct.tailleLogique = 1;
  			memcpy(listvirementStruct.listVirements, &listVirements, sizeof(listVirements));
  			swrite(sockfd, &listvirementStruct, sizeof(listvirementStruct));
  			sread(sockfd, &buffer, sizeof(buffer));
  			printf("%s\n", buffer);
  		} else {
  			printf("virement récurent de %d€ vers le compte %d\n", amount, accountNb);
  			// créer nv virement reccurent
  			swrite(pipefd[1], &virement, sizeof(Virement));

  		}
  		sclose(sockfd);
    }	else if (strcmp(strToken, "q") == 0) {
    	sclose(pipefd[1]);
    	exit(0);
    } else {
			perror("Bad arguments");
			printf("Entrez une commande > ");
    	continue;
  	}
  	printf("Entrez une commande > ");
	}
}
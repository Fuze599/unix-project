#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "utils_v1.h"

volatile sig_atomic_t end = 0;

void sigint_handler(int sig) {
	end = 1;
}

void virementsReccurents(void* pipefd, void* address, void* port){
	Virement all_virements[100]; 

	int indice = 0;
	int* ptnPipeFd = pipefd;
	int* ptnPort = (int*) port;
	Virement virementRecu;
	//char buffer[256];
	sclose(ptnPipeFd[1]);
	while(sread(ptnPipeFd[0], &virementRecu, sizeof(Virement))) {
		if (virementRecu.num_emeteur == -2) {
			break;
		} 
		else if(virementRecu.num_emeteur == -1) {
			if(indice == 0) continue;
			//Liste virements
			ListVirements listvirementStruct;
			listvirementStruct.tailleLogique = indice;
			listvirementStruct.isRecurrent = true;
			memcpy(listvirementStruct.listVirements, &all_virements, sizeof(all_virements));
			// Création socket pour interagir avec server
			int sockfd = ssocket();
			sconnect(address, *ptnPort, sockfd);
			swrite(sockfd, &listvirementStruct, sizeof(listvirementStruct));
			sclose(sockfd);
		}
		else{ // on ajoute à la liste
			all_virements[indice++] = virementRecu;
		}
	}
	sclose(ptnPipeFd[0]);
	exit(0);
}

void childTimer(void *delay, void *pipefd) {
	/*sigset_t set;
	ssigemptyset (&set);
	sigaddset(&set, SIGINT);*/
	ssigaction(SIGINT, sigint_handler);

	int* ptnPipeFd = pipefd;
	sclose(ptnPipeFd[0]);
	int* ptn = (int*) delay;
	Virement fakeVirement;
	fakeVirement.num_emeteur = -1;
	while(!end){
		sleep(*ptn);
		swrite(ptnPipeFd[1], &fakeVirement, sizeof(int));
	}
	sclose(ptnPipeFd[1]);
	exit(0);
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

  int childTimerId = fork_and_run2(childTimer, &delay, &pipefd);
  int childVirementsReccurents = fork_and_run3(virementsReccurents, &pipefd, &address, &port);
  sclose(pipefd[0]);
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
  	buffer[strlen(buffer) - 1] = '\0';
    const char* separator = " ";
    char* strToken = strtok (buffer, separator);

    if (strToken == NULL) {
    	perror("Bad arguments");
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
  			continue;
  		}
  		int sockfd = ssocket();
  		sconnect(address, port, sockfd);
			Virement virement = {num, accountNb, amount};
  		
  		if (strcmp(buffer, "+") == 0) {
  			Virement listVirements[100];
  			listVirements[0] = virement;
	  		ListVirements listvirementStruct;
	  		listvirementStruct.tailleLogique = 1;
			listvirementStruct.isRecurrent = false;
  			memcpy(listvirementStruct.listVirements, &listVirements, sizeof(listVirements));
  			swrite(sockfd, &listvirementStruct, sizeof(listvirementStruct));
  			sread(sockfd, &buffer, sizeof(buffer));
  			printf("%s\n", buffer);
  		} else {
  			// créer nv virement reccurent
  			swrite(pipefd[1], &virement, sizeof(Virement));
  		}
  		sclose(sockfd);
    }	else if (strcmp(strToken, "q") == 0) {
    	Virement virement = {-2, -2, -2};
    	
    	skill(childTimerId, SIGINT);
    	swaitpid(childTimerId, NULL, 0);
    	swrite(pipefd[1], &virement, sizeof(Virement));
    	swaitpid(childVirementsReccurents, NULL, 0);
    	sclose(pipefd[1]);
    	printf("fin\n");
    	exit(0);
    } else {
			perror("Bad arguments");
    	continue;
  	}
 }
}
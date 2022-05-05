#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "utils_v1.h"

void childTimer() {
	//dans le main il faut ajouter :
	

}

int main(int argc, char **argv) {
  if (argc != 5) {
  	perror("Not enough args");
  	exit(1);
  }

  char* address = argv[1];
  int port = atoi(argv[2]), num = atoi(argv[3]), delay = atoi(argv[4]);

  printf("%d\n", delay);

  int childTimerId = fork_and_run0(childTimer);
  printf("%d\n", childTimerId);

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

  		if (strcmp(buffer, "+") == 0) {
  			Virement virement = {num, accountNb, amount};
  			Virement listVirements[100];
  			listVirements[0] = virement;
  			ListVirements listvirementStruct;
  			listvirementStruct.tailleLogique = 1;

  			memcpy(listvirementStruct.listVirements, &listVirements, sizeof(listVirements));
  			swrite(sockfd, &listvirementStruct, sizeof(listvirementStruct));
  			sread(sockfd, &buffer, sizeof(buffer));
  			printf("%s\n", buffer);
  		} else {
  			printf("virement récurent de %d€ vers le compte %d\n", amount, accountNb);
  		}
  		sclose(sockfd);
    }	else if (strcmp(strToken, "q") == 0) {
    	exit(0);
    } else {
			perror("Bad arguments");
			printf("Entrez une commande > ");
    	continue;
  	}
  	printf("Entrez une commande > ");
	}
}
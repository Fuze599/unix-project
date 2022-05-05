#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "utils_v1.h"
void virementsReccurents(void* pipefd){
	int* ptnPipeFd = pipefd;
	int intVal = 0;
	while(sread(ptnPipeFd[0], &intVal, sizeof(int)) && intVal == 1){
		if(intVal == 1){
			printf("il faut exécuter les virements réccurents ici\n");
			intVal = 0;
		}
	}
}

void childTimer(void *delay, void *pipefd) {
	int* ptnPipeFd = pipefd;
	int* ptn = (int*) delay;
	int one = 1;
	while(1){
		sleep(*ptn);
		swrite(ptnPipeFd[1], &one, sizeof(int));
	}

}

int main(int argc, char **argv) {
  if (argc != 5) {
  	perror("Not enough args");
  	exit(1);
  }

  //char* address = args[1];
  int port = atoi(argv[2]), num = atoi(argv[3]), delay = atoi(argv[4]);
  //create a pipe
  int pipefd[2];
  spipe(pipefd);

  int childTimerId = fork_and_run2(childTimer, &delay, &pipefd);
  int childVirementsReccurents = fork_and_run1(virementsReccurents, &pipefd);
  printf("%d, %d, %d, %d, %d \n", childTimerId, childVirementsReccurents, port, num, delay);


  printf("Entrez une commande > ");
  char buffer[256];
  while (fgets(buffer, 256, stdin) != NULL) {
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
  		if (strcmp(buffer, "+") == 0) {
  			printf("virement unique de %d€ vers le compte %d\n", amount, accountNb);
  		} else {
  			printf("virement récurent de %d€ vers le compte %d\n", amount, accountNb);
  		}
    }	else if (strcmp(strToken, "q") == 0) {
    	exit(0);
    } else {
			perror("Bad arguments");
    	continue;
  	}
  	printf("Entrez une commande > ");
	}
	// on ferme le pipe côté lecture et écriture
	sclose(pipefd[0]);
	sclose(pipefd[1]);
}
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "utils_v1.h"

void childTimer() {

}

int main(int argc, char **argv) {
  if (argc != 5) {
  	perror("Not enough args");
  	exit(1);
  }

  //char* address = args[1];
  //int port = atoi(args[2]), num = atoi(args[3]), delay = atoi(args[4]);

  int childTimerId = fork_and_run0(childTimer);
  printf("%d\n", childTimerId);

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
}
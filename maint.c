#include <stdlib.h>

#include "utils.h"
#include "utils_v1.h"

#define PERM 0666

int main(int argc, char **argv) {
  if (argc < 2) {
  	perror("Not enough args");
  	exit(1);
  }

  int type = atoi(argv[1]);

  switch(type) {
  	case 1:
	  	int listAccount[NB_CLIENT];
	  	sem_create(SHM_SEM_KEY, 1, PERM, 1);
	  	sshmget(SHM_SEM_KEY, sizeof(listAccount), IPC_CREAT | PERM);
  		break;
  	case 2:
  		sshmdelete(SHM_SEM_KEY);
  		sem_delete(SHM_SEM_KEY);
  		break;
  	case 3:
  		break;
  	default:
  		exit(1);
  }
}

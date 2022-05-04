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
  if (type == 1) {
	sem_create(SHM_SEM_KEY, 1, PERM, 1);
	sshmget(SHM_SEM_KEY, NB_CLIENT * sizeof(int), IPC_CREAT | PERM);
  } else if (type == 2) {
  	int shm_id = sshmget(SHM_SEM_KEY, NB_CLIENT * sizeof(int), IPC_CREAT | PERM);
	sshmdelete(shm_id);
	int sem_id = sem_get(SHM_SEM_KEY, 1);
  	sem_delete(sem_id);
  } else if (type == 3) {

  } else {
  	perror("Bad argument");
   	exit(1);
  }
}

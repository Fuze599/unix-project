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

	int sockfd = initSocketServer(atoi(argv[1]));
	printf("Le serveur tourne sur le port : %i \n", sockfd);
  

  	sclose(sockfd);
}

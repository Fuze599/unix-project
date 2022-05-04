#ifndef _MESSAGES_H_
#define _MESSAGES_H_

// server
#define SERVER_PORT 	6666
#define SERVER_IP		"127.0.0.1"  /* localhost */
#define MAX_PSEUDO 		256

// semaphore / shared memory
#define SHM_SEM_KEY   666
#define NB_CLIENT 1000

#define INSCRIPTION_REQUEST 10
#define INSCRIPTION_OK		11
#define INSCRIPTION_KO 		12

/* struct message used between server and client */
typedef struct {
  char messageText[MAX_PSEUDO];
  int code;
} StructMessage;
#endif

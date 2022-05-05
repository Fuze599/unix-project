#ifndef _MESSAGES_H_
#define _MESSAGES_H_

// semaphore / shared memory
#define SHM_KEY   444
#define SEM_KEY   666
#define NB_CLIENT 1000
#define BUFFER_SIZE 256

typedef struct {
  int num_emeteur;
  int num_beneficiaire;
  int montant;
} Virement;

typedef struct {
  Virement listVirements[100];
  int tailleLogique;
} ListVirements;
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "pile.h"

// Définition de fonctions

Pile initPile () {
   return NULL;
}

bool pileVide (Pile p) {
   return (p == NULL);
}

bool push (Pile *p, int n) {
   Noeud *nv;
   if ((nv = (Noeud*)malloc(sizeof(Noeud))) == NULL){
      return false;
   }

   nv->val = n;
   nv->svt = *p;
   *p = nv;
   return true;
}

int pop (Pile *p) {
   /*
     if (pileVide(*p))
        return INT_MAX;
   */
   int ret = (*p)->val;
   Noeud *ptr = *p;
   *p = (*p)->svt;
   free(ptr);
   return ret;
}

void viderPile (Pile *p) {
   Noeud *ptr;
   while (!pileVide(*p)){
      ptr = *p;
      *p = (*p)->svt;
      free(ptr);
   }
   *p = NULL;
}

void afficherPile (Pile p) {
   if (pileVide(p)){
      printf("La pile est vide\n");
      return;
   }

   printf("Voici la pile : \n");
   for( ; p != NULL; p=p->svt){
      printf("\t%d\n", p->val);
   }
}

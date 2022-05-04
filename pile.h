#include <stdbool.h>

#ifndef _PILE_H_
#define _PILE_H_

// Définition de types

typedef struct Noeud {
    int val;
    struct Noeud *svt;
} Noeud;

typedef Noeud* Pile;

// Déclaration de fonctions

/* Initialise une pile d'entiers
 * RES: renvoie une pile vide
 */
Pile initPile ();

/* Teste si une pile est vide
 * PRE: p: pile d'entiers
 * RES: renvoie vrai si p est vide; faux sinon
 */
bool pileVide (Pile p);

/* Empile un entier sur une pile
 * PRE: p: pile d'entiers
 *      n: valeur entière à empiler
 * POST: n a été placé au sommet de la pile p
 * RES: renvoie vrai si n a bien été empilé; faux sinon
 */
bool push (Pile* p, int n);

/* Dépile un entier d'une pile
 * PRE: p: pile d'entiers non vide
 * POST: la valeur au sommet de la pile p a été dépilée
 * RES: renvoie la valeur dépilée du sommet de la pile p
 */
int pop (Pile* p);

/* Vide une pile d'entiers
 * PRE: p: pile d'entiers
 * POST: la pile p a été vidée
 */
void viderPile (Pile* p);

/* Affiche le contenu d'une pile d'entiers
 * PRE: p: pile d'entiers
 * POST: affiche le contenu de la pile p
 */
void afficherPile (Pile p);

#endif
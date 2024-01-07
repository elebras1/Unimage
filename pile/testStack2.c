/* -------------------------------------------------------------
   Exemple de programme de test pour les fonctions de gestion 
	de pile
   Dans le mesure du possible le test doit etre automatisé et 
	controler lui-même si les resultats sont conformes 
	a ceux attendu
   Il faur tester les cas standards (empilement sur un pile non pleine par exemple)
	et les cas limites (depiler sur une pile vide par exemple)
   Le test affiche les problèmes, pas les succes
   -------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "stack2.h"

const int taille=5;

int main() {
	my_stack_t * pile;
	int nb;
	int v1=10, v2=20, v3=30, v4=40, v5=50, v6=60;
	void *p;

	pile=stack_create(taille);
	if ( pile==NULL ) {
		fprintf(stderr, "Erreur lors de la creation de la pile (%s) \n", strerror(errno));
		return 1;
	}

	nb= stack_mem_available(pile);
	if ( nb != taille ) {
		fprintf(stderr, "stack_mem_available: attendu %d obtenu %d \n", taille, nb);
		return 2;
	}

	nb= stack_mem_used(pile);
	if ( nb != 0 ) {
		fprintf(stderr, "stack_mem_used: attendu %d obtenu %d \n", 0, nb);
		return 3;
	}

	// stockage valeurs
	if ( stack_push(pile, &v1) ) {
		fprintf(stderr, "stack_push: la pile ne dvrait pas etre pleine\n"); 
		return 4;
	}
		
	nb=stack_has_value_available(pile);
	if ( nb != 1 ) {
		fprintf(stderr, "stack_has_value_available: presence de valeurs non detectee\n");
		return 5;
	}
		
	if ( stack_push(pile, &v2) ) {
		fprintf(stderr, "stack_push: la pile ne dvrait pas etre pleine\n"); 
		return 6;
	}

	nb= stack_mem_used(pile) + stack_mem_available(pile);
	if ( nb != 5 ) {
		fprintf(stderr, "used+available != %d \n", 5);
		return 7;
	}
	
	// depilement
	p=stack_pop(pile);
	if (p==NULL || *(int*)p!=20 ) {
		fprintf(stderr, "v2 non retrouvé par depilement adresse %p attendu %p obtenu\n", &v2, p);
		return 8;
	}
	
	p=stack_pop(pile);
	if (p==NULL || *(int*)p!=10 ) {
		fprintf(stderr, "v1 non retrouvé par depilement adresse %p attendu %p obtenu\n", &v1, p);
		return 9;
	}

	stack_init(pile);
	nb= stack_mem_available(pile);
	if ( nb != taille ) {
		fprintf(stderr, "stack_init: pile non vide attendu %d places obtenu %d \n", taille, nb);
		return 10;
	}
	
	if  ( ! stack_is_empty(pile) )	{
		fprintf(stderr, "stack_is_empty: pile vide non detectee\n");
		return 11;
	}


	// cas limites
	// depiler sur une pile vide
	if ( stack_pop2(pile, &p)==0 ) {
		fprintf(stderr, "stack_pop2 : pile vide non detectee\n");
		return 12;
	}
		
	(void)stack_push(pile, &v1);
	(void)stack_push(pile, &v2);
	(void)stack_push(pile, &v3);
	(void)stack_push(pile, &v4);
	(void)stack_push(pile, &v5);
	if ( stack_push(pile, &v6)!=-1 )  {
		fprintf(stderr, "stack_push : pile pleine non detectee\n");
		return 13;
	}


	// iterateur
	stack_iterator_t * it=stack_iterator_create(pile);
	if ( it==NULL ) {
                fprintf(stderr, "Erreur lors de la creation de literateur pile (%s) \n", strerror(errno));
                return 14;
        }

	int err=0;
	
	p=stack_iterator_next(it );
	if (*(int*)p != v1 ) {
		fprintf(stderr, "stack_pop : attendu %d obtenu %d\n", v1, *(int*)p);
		return 15;
	}

	nb=*(int*)p;
	// diff 10 entre 2 vars
	while ( stack_iterator_next2(it, &p) ) {
		if (*(int*)p != nb+10 )  {
			fprintf(stderr," stack_next2 : attendu valeur %d obtenu %d\n", *(int*)p, nb+10);
			err=1;
		}
		nb=nb+10;
	}
	if (err) {
		 fprintf(stderr,"stack_iterator : etat pile incorrect");
		 return 16;
	}


	stack_remove(pile);
	
	return 0;
		
	
}

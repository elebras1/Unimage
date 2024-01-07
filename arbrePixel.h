#ifndef _ARBRE_PIXEL_H_
#define _ARBRE_PIXEL_H_

#include <glib.h>
#include <stdio.h>
#include "pile/stack2.h"

void couleurMoyenneEnfants(GNode *noeud, my_stack_t *pile);

void calculerCouleur(int x, int y, uint8_t *rouge, uint8_t *vert, uint8_t *bleu);

void initCouleurZPixelNoeud(GNode *noeud, uint8_t rouge, uint8_t vert, uint8_t bleu);

void couleurDegradationZPixel(struct Zpixel *zpixel, int x, int y, int taille);

void adapterTailleZpixel(struct Image *image, int *taille);

void parcoursProfondeur(GNode *noeud, struct Image *image, int degradation);

GNode *arbreZpixelConstruction(struct Image *image, int x, int y, int taille, void (*calculDegradation)(GNode *noeud, my_stack_t *pile));

GNode *construireArbreZPixel(struct Image *image, int x, int y, int taille, void (*calculDegradation)(GNode *noeud, my_stack_t *pile));

void tailleDegradation(GNode *noeud, my_stack_t *pile);

void luminositeDegradation(GNode *noeud, my_stack_t *pile);

void distanceDegradation(GNode *noeud, my_stack_t *pile);

#endif
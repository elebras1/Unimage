#include "zpixel.h"
#include "arbrePixel.h"
#include "pile/stack2.h"
#include <glib.h>
#include <stdio.h>
#include <math.h>

/* couleur pixel du noeud doit etre egal a la moyenne de ses enfants(4max)*/
void couleurMoyenneEnfants(GNode *noeud, my_stack_t *pile)
{
    int totalRouge = 0, totalVert = 0, totalBleu = 0, nbEnfants = stack_mem_used(pile);

    while (!stack_is_empty(pile))
    {
        struct Zpixel *enfant = stack_pop(pile);
        totalRouge += enfant->rouge;
        totalVert += enfant->vert;
        totalBleu += enfant->bleu;
    }

    struct Zpixel *zpixelParent = noeud->data;
    zpixelParent->rouge = totalRouge / nbEnfants;
    zpixelParent->vert = totalVert / nbEnfants;
    zpixelParent->bleu = totalBleu / nbEnfants;

    //printf("---couleurMoyenneEnfants : couleur Moyenne rouge %d / vert %d / bleu %d\n\n", zpixelParent->rouge, zpixelParent->vert, zpixelParent->bleu);
}

void calculerCouleur(int x, int y, uint8_t *rouge, uint8_t *vert, uint8_t *bleu)
{
    *rouge = x % 256;
    *vert = y % 256;
    *bleu = (x + y) % 256;
}

void initCouleurZPixelNoeud(GNode *noeud, uint8_t rouge, uint8_t vert, uint8_t bleu)
{
    struct Zpixel *zpixel = noeud->data;
    zpixel->rouge = rouge;
    zpixel->vert = vert;
    zpixel->bleu = bleu;
    //printf("taille 1 couleur : rouge :%d, vert :%d, bleu : %d, degra : %d\n", zpixel->rouge, zpixel->vert, zpixel->bleu, zpixel->facteurDegra);
}

void couleurDegradationZPixel(struct Zpixel *zpixel, int x, int y, int taille)
{
    zpixel->facteurDegra = (taille - 1) * (1 + abs(x - y));
    //printf("--- couleurDegradationZPixel : zpixel taille %d x :%d y:%d degradation :%d \n", zpixel->taille, zpixel->x, zpixel->y, zpixel->facteurDegra);
}

GNode *construireArbreZPixel(struct Image *image, int x, int y, int taille, void (*calculDegradation)(GNode *noeud, my_stack_t *pile))
{
    if (x < 0 || y < 0 || x >= image->largeur || y >= image->hauteur)
        return NULL;

    struct Zpixel *zpixel = createZPixel(taille, 0, 0, 0, x, y);
    GNode *noeud = g_node_new(zpixel);

    if (zpixel->taille == 1)
    {
        uint8_t rouge, vert, bleu;
        calculerCouleur(x, y, &rouge, &vert, &bleu);
        initCouleurZPixelNoeud(noeud, rouge, vert, bleu);
    }
    else
    {
        my_stack_t *pile = stack_create(4);
        
        GNode *filsGaucheHaut = construireArbreZPixel(image, x, y, taille / 2, calculDegradation);
        if(filsGaucheHaut != NULL) {
            g_node_append(noeud, filsGaucheHaut);
            stack_push(pile, filsGaucheHaut->data);
        }

        GNode *filsDroiteHaut = construireArbreZPixel(image, x + taille / 2, y, taille / 2, calculDegradation);
        if(filsDroiteHaut != NULL) {
            g_node_append(noeud, filsDroiteHaut);
            stack_push(pile, filsDroiteHaut->data);
        }

        GNode *filsGaucheBas = construireArbreZPixel(image, x, y + taille / 2, taille / 2, calculDegradation);
        if(filsGaucheBas != NULL) {
            g_node_append(noeud, filsGaucheBas);
            stack_push(pile, filsGaucheBas->data);
        }

        GNode *filsDroiteBas = construireArbreZPixel(image, x + taille / 2, y + taille / 2, taille / 2, calculDegradation);
        if(filsDroiteBas != NULL) {
            g_node_append(noeud, filsDroiteBas);
            stack_push(pile, filsDroiteBas->data);
        }
        couleurDegradationZPixel(zpixel, x, y, zpixel->taille);
        couleurMoyenneEnfants(noeud, pile);
        calculDegradation(noeud, pile);
        stack_remove(pile);
    }

    return noeud;
}

GNode *arbreZpixelConstruction(struct Image *image, int x, int y, int taille, void (*calculDegradation)(GNode *noeud, my_stack_t *pile))
{
    adapterTailleZpixel(image, &taille);
    x = 0;
    y = 0;

    return construireArbreZPixel(image, x, y, taille, calculDegradation); 
}

void adapterTailleZpixel(struct Image *image, int *taille)
{
    int max = image->largeur;
    if(max < image->hauteur)
        max = image->hauteur;

    int taille2 = 1;
    while(taille2 < max)
    {
        taille2 *= 2;
    }
    *taille = taille2;

    //printf("taille generer : %d\n", *taille);
}

void parcoursProfondeur(GNode *noeud, struct Image *image, int degradation)
{ 
    struct Zpixel *zpixel = noeud->data;
    if(zpixel->facteurDegra < degradation)
    {
        projection(image, zpixel);
        //printf("--zpixel taille : %d, x:%d, y:%d, degra:%d, rouge:%d, vert:%d, bleu:%d\n", zpixel->taille, zpixel->x, zpixel->y, zpixel->facteurDegra, zpixel->rouge, zpixel->vert, zpixel->bleu);
    }
    else
    {
        GNode *enfant = noeud->children;
        while (enfant != NULL)
        {
            parcoursProfondeur(enfant, image, degradation);
            enfant = enfant->next;
        }
    }
}

void tailleDegradation(GNode *noeud, my_stack_t *pile)
{
    (void) pile;
    struct Zpixel *zpixel = noeud->data;
    zpixel->facteurDegra += zpixel->taille - 1;
    //printf("tailleDegradation zpixel taille : %d\n", zpixel->taille);
}

void luminositeDegradation(GNode *noeud, my_stack_t *pile)
{
    stack_iterator_t *ite = stack_iterator_create(pile);
    struct Zpixel *zpixel = noeud->data;
    int nbEnfants = stack_mem_used(pile);

    while (nbEnfants > 0)
    {
        struct Zpixel *enfant = stack_iterator_next(ite);
        zpixel->facteurDegra += luminosite(enfant);
        //printf("luminositeDegradation = %d\n", luminosite(enfant));
        nbEnfants--;
    }
}

void distanceDegradation(GNode *noeud, my_stack_t *pile)
{
    stack_iterator_t *ite = stack_iterator_create(pile);
    struct Zpixel *zpixel = noeud->data;
    int nbEnfants = stack_mem_used(pile);

    struct Zpixel *enfant1 = stack_iterator_next(ite);
    while (nbEnfants > 1)
    {
        struct Zpixel *enfant2 = stack_iterator_next(ite);
        zpixel->facteurDegra += distance(enfant1, enfant2);
        //printf("distanceDegradation = %d\n", distance(enfant1, enfant2));
        enfant1 = enfant2;
        nbEnfants--;
    }
}
#include "zpixel.h"
#include <stdio.h>
#include <stdlib.h>
#include "pile/stack2.h"
#include "arbrePixel.h"
#include "interface.h"
#include <glib.h>

void test1() 
{
    // Test de la fonction createPixel
    struct Zpixel *zpixelN = createZPixel(3, 0, 0, 0, 3, 2);
    struct Zpixel *zpixelB = createZPixel(5, 255, 255, 255, 7, 4);
    struct Zpixel *zpixel = createZPixel(5, 20, 30, 40, 2, 3);
    if (zpixelN == NULL || zpixelB == NULL || zpixel == NULL) 
    {
        printf("---Test fonction createPixel\n");
        printf("Problème lors de la creation d'un Zpixel\n");
    }

    // Test de la saturation
    int sat1 = saturation(zpixelB);
    int sat2 = saturation(zpixel);
    if (sat1 != 0)
    {
        printf("---Test saturation\n");
        printf("Erreur saturation, res : %d, attendu : 0\n", sat1);
    }
    else if (sat2 != 85) 
    {
        printf("---Test saturation\n");
        printf("Erreur saturation, res : %d, attendu : 85\n", sat2);
    }

    // Test de la fonction distance
    int dist = distance(zpixel, zpixelB);
    if (dist != 389)
    {
        printf("---Test fonction de distance\n");
        printf("res : %d, attendu : 389\n", dist);
    }

    // Test de la fonction createImage
    struct Image *image = createImage(8, 8);
    if (image == NULL)
    {
        printf("---Test fonction createImage\n");
        printf("Problème lors de la creation de l'image\n");   
    }

    initBlackImage(image);

    // Test de la fonction setPixel
    printf("---Test fonction setPixel\n");
    int sp = setPixel(image, 0, 0, 10, 23, 18);
    if (sp == -1)
    {
        printf("Erreur du set\n");
    }

    showImage(image);

    // Test de la fonction getPixel
    uint8_t rouge;
    uint8_t vert;
    uint8_t bleu;
    int gp = getPixel(image, 0, 0, &rouge, &vert, &bleu);
    if (gp == -1)
    {
        printf("---Test fonction getPixel\n");
        printf("Le pixel ne respecte pas les dimensions de l'image.\n");
    }

    if (rouge + vert + bleu != 51)
    {
        printf("---Test fonction getPixel\n");
        printf("erreur lors de la récupérationdu pixel%d\n", rouge + vert + bleu);
    }

    // Test de la fonction projection
    printf("---Test fonction projection\n");
    projection(image, zpixel);
    showImage(image);
    projection(image, zpixelB);
    showImage(image);
}

void test2() 
{
    // Test fonction couleurMoyenneEnfants
    my_stack_t *pile = stack_create(4);
    struct Zpixel *enfant1 = createZPixel(1, 10, 20, 30, 0, 0);
    struct Zpixel *enfant2 = createZPixel(1, 20, 30, 40, 0, 0);
    stack_push(pile, enfant1);
    stack_push(pile, enfant2);
    struct Zpixel *zpixel = createZPixel(1, 0, 0, 0, 0, 0);
    GNode *n = g_node_new(zpixel);
    g_node_append(n, g_node_new(enfant1));
    g_node_append(n, g_node_new(enfant2));
    couleurMoyenneEnfants(n, pile);
    if(zpixel->rouge + zpixel->vert + zpixel->bleu != 75)
    {
        printf("Test fonction couleurMoyenneEnfants\n");
        printf("couleurs incorrecte\n");
    }


    // Test de la fonction calculerCouleur
    uint8_t rouge = 0, vert = 0, bleu = 0;
    int x = 4, y = 2;
    calculerCouleur(x, y, &rouge, &vert, &bleu);
    if(rouge + vert + bleu != 12)
    {
        printf("Test fonction calculerCouleur\n");
        printf("couleurs incorrecte\n");
    }


    // Test de la fonction couleurDegradationZPixel
    couleurDegradationZPixel(zpixel, x, y, 8);
    if(zpixel->facteurDegra != 21)
    {
        printf("Test fonction couleurDegradationZPixel\n");
        printf("facteur de dégradation incorrecte\n");
    }


    // Test de la fonction construireArbreZPixel
    printf("---Test fonction construireArbreZPixel\n");
    struct Image *image = createImage(8, 12);
    initBlackImage(image);
    printf("degradation 1 :\n");
    GNode *noeud = arbreZpixelConstruction(image, 0, 0, 5, tailleDegradation);
    if(noeud == NULL)
    {
        printf("echec creation de l'abre.\n");
    }
    else
    {
        //Test parcours profondeur
        parcoursProfondeur(noeud, image, 1);
        showImage(image);
    }

    struct Image *image2 = createImage(13, 12);
    initBlackImage(image2);
    printf("degradation 45 :\n");
    GNode *noeud2 = arbreZpixelConstruction(image2, -4, -8, 4, distanceDegradation);
    if(noeud == NULL)
    {
        printf("echec creation de l'abre.\n");
    }
    else
    {
        //Test parcours profondeur
        parcoursProfondeur(noeud2, image2, 45);
        showImage(image2);
    }
}

void test3()
{
    // initialisation donnees pour test
    my_stack_t *pile = stack_create(4);
    struct Zpixel *enfant1 = createZPixel(1, 10, 20, 30, 0, 0);
    struct Zpixel *enfant2 = createZPixel(1, 20, 30, 40, 0, 0);
    stack_push(pile, enfant1);
    stack_push(pile, enfant2);
    struct Zpixel *zpixel = createZPixel(8, 0, 0, 0, 0, 0);
    GNode *noeud = g_node_new(zpixel);
    g_node_append(noeud, g_node_new(enfant1));
    g_node_append(noeud, g_node_new(enfant2));


    //test de la fonction tailleDegradation
    tailleDegradation(noeud, pile);
    if(zpixel->facteurDegra != 7)
    {
        printf("Test fonction couleurMoyenneEnfants\n");
        printf("degradation a partir de la taille incorrecte, valeur : %d, valeur attendue : 7\n", zpixel->facteurDegra);
    }

    //test de la fonction luminositeDegradation
    zpixel->facteurDegra = 0;
    luminositeDegradation(noeud, pile);
    if(zpixel->facteurDegra != 50)
    {
        printf("Test fonction luminositeDegradation\n");
        printf("degradation a partir de la luminosite incorrecte, valeur : %d, valeur attendue : 50\n", zpixel->facteurDegra);
    }

    //test de la fonction distanceDegradation
    zpixel->facteurDegra = 0;
    distanceDegradation(noeud, pile);
    if(zpixel->facteurDegra != 17)
    {
        printf("Test fonction distanceDegradation\n");
        printf("degradation a partir de la distance incorrecte, valeur : %d, valeur attendue : 17\n", zpixel->facteurDegra);
    }
}

int main(int argc, char *argv[]) {
    test3();
    interface(argc, argv);
    printf("Fin des tests\n\n");

    return 0;
}

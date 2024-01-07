#include "zpixel.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Zpixel *createZPixel(int taille, uint8_t rouge, uint8_t vert, uint8_t bleu, int x, int y)
{
    struct Zpixel *zpixel = (struct Zpixel *)malloc(sizeof(struct Zpixel));
    if (zpixel == NULL)
    {
        return NULL;
    }

    zpixel->taille = taille;
    zpixel->rouge = rouge;
    zpixel->vert = vert;
    zpixel->bleu = bleu;
    zpixel->x = x;
    zpixel->y = y;

    return zpixel;
}

void destroyZpixel(struct Zpixel *zpixel)
{
    free(zpixel);
}

int minC(uint8_t rouge, uint8_t vert, uint8_t bleu)
{
    int min = rouge;

    if (vert < min)
    {
        min = vert;
    }

    if (bleu < min)
    {
        min = bleu;
    }

    return min;
}

int maxC(uint8_t rouge, uint8_t vert, uint8_t bleu)
{
    int max = rouge;

    if (vert > max)
    {
        max = vert;
    }

    if (bleu > max)
    {
        max = bleu;
    }

    return max;
}

int luminosite(struct Zpixel *zpixel)
{
    int max = maxC(zpixel->rouge, zpixel->vert, zpixel->bleu);
    int min = minC(zpixel->rouge, zpixel->vert, zpixel->bleu);

    int res = (max + min) / 2;

    return res;
}

int saturation(struct Zpixel *zpixel)
{
    int res = 0;

    int max = maxC(zpixel->rouge, zpixel->vert, zpixel->bleu);
    int min = minC(zpixel->rouge, zpixel->vert, zpixel->bleu);

    if (luminosite(zpixel) < 128)
    {
        if (max == 0 && min == 0)
            return 0;
        res = 255 * (max - min) / (max + min);
    }
    else
        res = 255 * (max - min) / (511 - (max - min));

    return res;
}

int distance(struct Zpixel *zpixel1, struct Zpixel *zpixel2)
{
    return sqrt((zpixel1->rouge - zpixel2->rouge) * (zpixel1->rouge - zpixel2->rouge) + (zpixel1->bleu - zpixel2->bleu) * (zpixel1->bleu - zpixel2->bleu) + (zpixel1->vert - zpixel2->vert) * (zpixel1->vert - zpixel2->vert));
}

struct Image *createImage(int hauteur, int largeur)
{
    struct Image *image = (struct Image *)malloc(sizeof(struct Image));
    if (image == NULL)
    {
        return NULL;
    }

    image->hauteur = hauteur;
    image->largeur = largeur;
    image->rowstride = largeur * 3 + 1;
    image->listePixel = (uint8_t *)malloc(sizeof(uint8_t) * hauteur * image->rowstride);
    if (image->listePixel == NULL)
    {
        return NULL;
    }

    return image;
}

void initBlackImage(struct Image *image)
{
    for(int i = 0; i < image->hauteur; i++)
    {
        for(int j = 0; j < image->largeur; j++)
        {
            setPixel(image, j, i, 0, 0, 0);
        }
        
    }
}

void showImage(struct Image *image)
{
    for(int i = 0; i < image->hauteur; i++)
    {
        for(int j = 0; j < image->largeur; j++)
        {
            uint8_t rouge, vert, bleu;
            getPixel(image, j, i, &rouge, &vert, &bleu);
            printf("(%3u, %3u, %3u) ", rouge, vert, bleu);
        }
        printf("\n");
    }
}


int getPixel(struct Image *image, int x, int y, uint8_t *rouge, uint8_t *vert, uint8_t *bleu)
{
    if (x < 0 || y < 0 || x >= image->largeur || y >= image->hauteur)
    {
        return -1;
    }

    int origine = image->rowstride * y + x * 3;

    *rouge = image->listePixel[origine];
    *vert = image->listePixel[origine + 1];
    *bleu = image->listePixel[origine + 2];

    return 1;
}

int setPixel(struct Image *image, int x, int y, uint8_t rouge, uint8_t vert, uint8_t bleu)
{
    if (x < 0 || y < 0 || x >= image->largeur || y >= image->hauteur)
    {
        return -1;
    }
    int origine = image->rowstride * y + x * 3;

    image->listePixel[origine] = rouge;
    image->listePixel[origine + 1] = vert;
    image->listePixel[origine + 2] = bleu;
    //printf("rouge :%d, vert :%d, bleu :%d\n", image->listePixel[origine], image->listePixel[origine + 1], image->listePixel[origine + 2]);

    return 1;
}

int projection(struct Image *image, struct Zpixel *zpixel)
{
    for (int i = 0; i < zpixel->taille; i++)
    {
        for (int j = 0; j < zpixel->taille; j++)
        {
            setPixel(image, j + zpixel->x, i + zpixel->y, zpixel->rouge, zpixel->vert, zpixel->bleu);
        }
    }
    return 1;
}
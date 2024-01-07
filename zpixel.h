#ifndef _ZPIXEL_H_
#define _ZPIXEL_H_
#include <stdint.h>

struct Zpixel
{
    int taille;
    uint8_t rouge;
    uint8_t vert;
    uint8_t bleu;
    int x;
    int y;
    int facteurDegra;
}; 

struct Zpixel *createZPixel(int taille, uint8_t rouge, uint8_t vert, uint8_t bleu, int x, int y);
 
void destroyZpixel(struct Zpixel *zpixel);

int luminosite(struct Zpixel *zpixel);

int saturation(struct Zpixel *zpixel);

int distance(struct Zpixel *zpixel1, struct Zpixel *zpixel2);

struct Image
{
    int largeur;
    int hauteur;
    uint8_t *listePixel;
    int rowstride;
};

void showImage(struct Image *image);

void initBlackImage(struct Image *image);

struct Image *createImage(int hauteur, int largeur);

int getPixel(struct Image *image, int x, int y, uint8_t *rouge, uint8_t *vert, uint8_t *bleu);

int setPixel(struct Image *image, int x, int y, uint8_t rouge, uint8_t vert, uint8_t bleu);

int projection(struct Image *image, struct Zpixel *zpixel);
#endif

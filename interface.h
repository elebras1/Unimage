#ifndef _ARBRE_INTERFACE_H_
#define _ARBRE_INTERFACE_H_

#include <gtk/gtk.h>

void interface(int argc, char *argv[]);

void okButtonClick(GtkButton *button, gpointer user_data) ;

void aProposMenuItem(GtkMenuItem *menuitem, gpointer user_data);

void degradeImage(GtkBuilder *builder);

void importerImage(GtkMenuItem *menuitem, gpointer user_data);

struct Image *gtkImageToStructImage(GdkPixbuf *pixbuf, double degradation, int fctDegradation);

void structImageToGtkImage(GtkImage *gtkImage, struct Image *image);

#endif
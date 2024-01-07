#include "zpixel.h"
#include "arbrePixel.h"
#include <gtk/gtk.h>

void okButtonClick(GtkButton *button, gpointer userData) {
    (void) button;
    GtkWidget *windowAPropos = GTK_WIDGET(userData);
    gtk_widget_hide(windowAPropos);
}

void aProposMenuItem(GtkMenuItem *menuItem, gpointer userData) {
    (void) menuItem;
    GtkBuilder *builder = (GtkBuilder *)userData;
    GtkWidget *windowAPropos = GTK_WIDGET(gtk_builder_get_object(builder, "aPropos"));
    GtkWidget *okButton = GTK_WIDGET(gtk_builder_get_object(builder, "finApropos"));

    g_signal_connect(okButton, "clicked", G_CALLBACK(okButtonClick), windowAPropos);

    gtk_widget_show_all(windowAPropos);
}

struct Image *gtkImageToStructImage(GdkPixbuf *pixbuf, double degradation, int fctDegradation) {
    printf("degradation : %f\n", degradation);
    printf("fctDegradation : %d\n", fctDegradation);
    int largeur = gdk_pixbuf_get_width(pixbuf);
    int hauteur = gdk_pixbuf_get_height(pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

    struct Image *image = createImage(hauteur, largeur);

    for (int h = 0; h < hauteur; h++) {
        for (int l = 0; l < largeur; l++) {
            guchar *pixel = pixels + h * rowstride + l * 3;
            uint8_t rouge = pixel[0];
            uint8_t vert = pixel[1];
            uint8_t bleu = pixel[2];

            setPixel(image, l, h, rouge, vert, bleu);
        }
    }

    GNode *racine;
    if(fctDegradation == 1)
        racine = arbreZpixelConstruction(image, 0, 0, largeur, tailleDegradation);
    else if(fctDegradation == 2)
        racine = arbreZpixelConstruction(image, 0, 0, largeur, luminositeDegradation);
    else if(fctDegradation == 3)
        racine = arbreZpixelConstruction(image, 0, 0, largeur, distanceDegradation);
    parcoursProfondeur(racine, image, degradation);

    return image;
}

void structImageToGtkImage(GtkImage *gtkImage, struct Image *image) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data((guchar*)image->listePixel, GDK_COLORSPACE_RGB, FALSE, 8, image->largeur, image->hauteur, image->rowstride, NULL, NULL);
    gtk_image_set_from_pixbuf(gtkImage, pixbuf);
    g_object_unref(pixbuf);
}


void degradeImage(GtkBuilder *builder) {
    GtkRadioButton *taille = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "taille"));
    GtkRadioButton *luminosite = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "luminosite"));
    GtkRadioButton *distance = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "distance"));

    GtkScale *degradation = GTK_SCALE(gtk_builder_get_object(builder, "degradation"));
    double degradationValeur = gtk_range_get_value(GTK_RANGE(degradation));

    GtkImage *imageOriginal = GTK_IMAGE(gtk_builder_get_object(builder, "imageOriginal"));
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(imageOriginal);

    struct Image *image;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(taille))) {
        image = gtkImageToStructImage(pixbuf, degradationValeur, 1);
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(luminosite))) {
        image = gtkImageToStructImage(pixbuf, degradationValeur, 2);
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(distance))) {
        image = gtkImageToStructImage(pixbuf, degradationValeur, 3);
    }

    GtkImage *gtkImage = GTK_IMAGE(gtk_builder_get_object(builder, "imageDegrade"));
    structImageToGtkImage(gtkImage, image);
}

void importerImage(GtkMenuItem *menuItem, gpointer userData) {
    (void)menuItem;
    GtkBuilder *builder = (GtkBuilder *)userData;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File", NULL, action, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        GtkWidget *imageOriginal = GTK_WIDGET(gtk_builder_get_object(builder, "imageOriginal"));
        gtk_image_set_from_file(GTK_IMAGE(imageOriginal), filename);

        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(imageOriginal));
        int width = gdk_pixbuf_get_width(pixbuf);
        int height = gdk_pixbuf_get_height(pixbuf);
        int totalPixels = width * height;

        GtkWidget *label = GTK_WIDGET(gtk_builder_get_object(builder, "tailleImageOriginal"));
        char *label_text = g_strdup_printf("%d pixels", totalPixels);
        gtk_label_set_text(GTK_LABEL(label), label_text);

        degradeImage(builder);

        g_free(label_text);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void interface(int argc, char *argv[]) {
    GtkWidget *window;
    GtkBuilder *builder;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    gtk_builder_add_from_file(builder, "interface.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    GtkWidget *about_menu_item = GTK_WIDGET(gtk_builder_get_object(builder, "itemApropos"));
    GtkWidget *quit_menu_item = GTK_WIDGET(gtk_builder_get_object(builder, "itemQuitter"));
    GtkWidget *import_menu_item = GTK_WIDGET(gtk_builder_get_object(builder, "itemImporter"));

    /*GtkRadioButton *taille = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "taille"));
    GtkRadioButton *luminosite = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "luminosite"));
    GtkRadioButton *distance = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "distance"));

    GtkScale *degradation = GTK_SCALE(gtk_builder_get_object(builder, "degradation"));*/

    g_signal_connect(about_menu_item, "activate", G_CALLBACK(aProposMenuItem), builder);
    g_signal_connect(quit_menu_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(import_menu_item, "activate", G_CALLBACK(importerImage), builder);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /*g_signal_connect(taille, "toggled", G_CALLBACK(degradeImage(builder)), builder);
    g_signal_connect(luminosite, "toggled", G_CALLBACK(degradeImage(builder)), builder);
    g_signal_connect(distance, "toggled", G_CALLBACK(degradeImage(builder)), builder);
    g_signal_connect(degradation, "value-changed", G_CALLBACK(degradeImage(builder)), builder);*/

    gtk_widget_show_all(window);

    gtk_main();
}
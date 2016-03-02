#include <gtk/gtk.h>
#include <stdio.h>
#include "init.h"
#include <math.h>

GtkLabel       **labels;
GtkBuilder      *builder; 
GtkWidget       *window;

int main(int argc, char *argv[])
{


    char str[4];

    gchar *label4text = "Hello";
    gtk_init(&argc, &argv);
    labels = initgui(&window);
    tick(labels);
    gtk_main();

    return 0;
}

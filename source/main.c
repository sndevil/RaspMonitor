#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "init.h"
#include "functions.h"

GtkLabel       **labels;
GtkNotebook     *notebook;
GtkBuilder      *builder; 
GtkWidget       *window;
GtkStatusbar    *statbar;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    gtk_init(&argc, &argv);
    labels = initgui(&window,&notebook);

    tick(labels);
    gtk_main();

    return 0;
}

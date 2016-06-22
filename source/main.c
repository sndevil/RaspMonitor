#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>

#include "init.h"
#include "functions.h"
#include "serial.h"

GtkLabel       **labels;
GtkNotebook     *notebook;
GtkBuilder      *builder; 
GtkWidget       *window;
GtkStatusbar    *statbar;

int USB;
char* input;
int main(int argc, char *argv[])
{
    srand(time(NULL));
    gtk_init(&argc, &argv);
    labels = initgui(&window,&notebook);
    
    g_timeout_add_seconds(1,tick,labels);
    g_timeout_add(100,keyboardtick,labels);
    //g_timeout_add_seconds(1,togglestatus,NULL);
    gtk_main();

    return 0;
}

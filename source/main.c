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
    
    //USB = SerialOpen("/dev/ttyUSB0",(speed_t)B115200);
    //input = SerialRead(USB);
    //printf("Successful: %s\n",input);
    //tick(labels);
    g_timeout_add_seconds(1,tick,labels);
    gtk_main();

    return 0;
}

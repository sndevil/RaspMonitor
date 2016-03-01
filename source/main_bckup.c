#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    GtkWidget       *notebook;
    GtkWidget       *table;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    
    gtk_builder_add_from_file (builder, "visual/Satmonitor.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Mainwindow"));
    gtk_builder_connect_signals(builder, NULL);

    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_table_attach_defaults(GTK_TABLE(table), notebook, 0,6,0,1);
    gtk_widget_show(notebook);

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

void on_window_main_destroy()
{
    gtk_main_quit();
}


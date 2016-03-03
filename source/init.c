#include <gtk/gtk.h>
#include "functions.h"

GtkLabel** initgui(GtkWidget *window, GtkNotebook **notebook)
{

  GtkBuilder  *builder;
  GtkLabel **labels;
  GtkWidget *page;
  //GtkWidget *button;
    builder = gtk_builder_new();
    labels = malloc(30 * sizeof(GtkLabel*));
    gtk_builder_add_from_file (builder, "visual/Satmonitor.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Mainwindow"));
    labels[0] = GTK_LABEL(gtk_builder_get_object(builder, "inputpowerval"));
    labels[1] = GTK_LABEL(gtk_builder_get_object(builder, "duplerfreqval"));
    labels[2] = GTK_LABEL(gtk_builder_get_object(builder, "carrierlockval"));
    labels[3] = GTK_LABEL(gtk_builder_get_object(builder, "timinglockval"));
    labels[4] = GTK_LABEL(gtk_builder_get_object(builder, "Ebnval"));
    labels[5] = GTK_LABEL(gtk_builder_get_object(builder, "BERval"));
    labels[6] = GTK_LABEL(gtk_builder_get_object(builder, "receiverateval"));
    labels[7] = GTK_LABEL(gtk_builder_get_object(builder, "processtempval"));
    labels[8] = GTK_LABEL(gtk_builder_get_object(builder, "controltempval"));
    labels[9] = GTK_LABEL(gtk_builder_get_object(builder, "racktempval"));
    labels[10] = GTK_LABEL(gtk_builder_get_object(builder, "patempval"));
    labels[11] = GTK_LABEL(gtk_builder_get_object(builder, "Powerval"));
    labels[12] = GTK_LABEL(gtk_builder_get_object(builder, "Sendpowerval"));
    labels[13] = GTK_LABEL(gtk_builder_get_object(builder, "Sendrateval"));
    labels[14] = GTK_LABEL(gtk_builder_get_object(builder, "Receiverateval"));
    labels[15] = GTK_LABEL(gtk_builder_get_object(builder, "Sendfreqval"));
    labels[16] = GTK_LABEL(gtk_builder_get_object(builder, "Receivefreqval"));
    
    //button = GTK_BUTTON(gtk_builder_get_object(builder, "Onoffsetbtn"));
    //page = GTK_WIDGET(gtk_builder_get_object(builder, "notebookpage"));
    //*notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "notebook"));
    //gtk_notebook_append_page(*notebook,page,gtk_label_new("tab2"));
//printf("in init, notebook is: %d\n", *notebook);
    

    gtk_builder_connect_signals(builder, NULL);
    //g_signal_connect (button, "clicked",
    //		      G_CALLBACK(on_Onoffsetbtn_clicked),(gpointer)"hello");
    g_object_unref(builder);
    gtk_widget_show_all(window);
    return labels;
}

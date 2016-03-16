#include <gtk/gtk.h>
//#include "functions.h"

GtkLabel** initgui(GtkWidget *window, GtkNotebook **notebook)
{

  GtkBuilder  *builder;
  GtkLabel **labels;
  GtkWidget *page;
  GtkContainer *viewport;
  GtkTextBuffer *textbuffer;
  GtkTextIter textiter;

  //GtkNotebook *notebook;

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
    labels[17] = GTK_LABEL(gtk_builder_get_object(builder, "humidityval"));
    labels[18] = GTK_LABEL(gtk_builder_get_object(builder, "voltageval"));
    labels[19] = GTK_LABEL(gtk_builder_get_object(builder, "currentval"));
    labels[20] = GTK_LABEL(gtk_builder_get_object(builder, "systemstatval"));
    labels[21] = GTK_LABEL(gtk_builder_get_object(builder, "carriererrorval"));
    labels[22] = GTK_LABEL(gtk_builder_get_object(builder, "externalfreqlockstatval"));
    labels[23] = GTK_LABEL(gtk_builder_get_object(builder, "viterbistatval"));
    labels[24] = GTK_LABEL(gtk_builder_get_object(builder, "timinglooperrorval"));
    labels[25] = GTK_LABEL(gtk_builder_get_object(builder, "errorsignalstatval"));
    labels[26] = GTK_LABEL(gtk_builder_get_object(builder, "staticons"));
    labels[27] = GTK_WIDGET(gtk_builder_get_object(builder, "Statusbar"));
    labels[28] = GTK_WIDGET(gtk_builder_get_object(builder, "loading"));

    labels[29] = GTK_WIDGET(gtk_builder_get_object(builder, "box3"));//"Logscrolledwindow"));
    viewport = GTK_CONTAINER(gtk_builder_get_object(builder,"viewport4"));
    g_object_ref (labels[29]);
    gtk_container_remove (viewport,labels[29]);
    
    labels[30] = gtk_builder_get_object(builder, "logbuffer");
    
    //textbuffer = gtk_builder_get_object(builder, "logbuffer");
    // gtk_text_buffer_get_start_iter(labels[30],&textiter);
    //gtk_text_buffer_insert(labels[30],&textiter,"hey",3);

    gtk_statusbar_push (GTK_STATUSBAR(labels[27]),1,"");
    
    *notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder,"notebook"));
    gtk_notebook_insert_page (*notebook,
			      labels[29],
			      gtk_label_new("Log"), 2);
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show_all(window);
    return labels;
}

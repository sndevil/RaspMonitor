#include <gtk/gtk.h>
GtkLabel** initgui(GtkWidget *window)
{

  GtkBuilder  *builder;
  GtkLabel **labels;
    builder = gtk_builder_new();
    labels = malloc(10 * sizeof(GtkLabel*));
    gtk_builder_add_from_file (builder, "visual/Satmonitor.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Mainwindow"));
    labels[0] = GTK_LABEL(gtk_builder_get_object(builder, "label4"));
    labels[1] = GTK_LABEL(gtk_builder_get_object(builder, "label5"));
    labels[2] = GTK_LABEL(gtk_builder_get_object(builder, "label6"));
    labels[3] = GTK_LABEL(gtk_builder_get_object(builder, "label7"));
    labels[4] = GTK_LABEL(gtk_builder_get_object(builder, "label8"));

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show_all(window);
    return labels;
}

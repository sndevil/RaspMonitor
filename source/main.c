#include <gtk/gtk.h>
#include <stdio.h>

GtkLabel       *label;
int counter = 0;
int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;

    char str[4];

    //Glib::ustring text = "hello";
    gchar *label4text = "Hello";
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    
    gtk_builder_add_from_file (builder, "visual/Satmonitor.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Mainwindow"));
    label = GTK_LABEL(gtk_builder_get_object(builder, "label4"));

    //gtk_label_set_text(label,"hello");
    //gtk_widget_set_tooltip_text(label,label4text);
    //label.set_text("text");
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show_all(window);

    tick(label);
    //gtk_label_set_text(label,"Hello");
    //while(1){
      gtk_main();
      //}

      //    gtk_main();
    return 0;
}

void on_window_main_destroy()
{
    gtk_main_quit();
}

void tick(gpointer data)
{
  char str[5];
  g_timeout_add_seconds(1,tick,data);
  snprintf(str,5,"%d",counter);
  gtk_label_set_text(label,str);
  counter++;
  if (counter > 1000)
    counter = 0;
}

#include <gtk/gtk.h>

int counter = 0;

void on_window_main_destroy()
{
    gtk_main_quit();
}

void str(char* out, int i)
{
  snprintf(out,10,"%d",i);
}

void strdbl(char* out, double i)
{
  snprintf(out,10,"%f",i);
}

void tick(GtkLabel** labels)
{
  g_timeout_add_seconds(1,tick,labels);
  changeLabel(labels[0],counter);
  changeLabeldbl(labels[1],0.261*counter+20);
  changeLabel(labels[2],counter+523);
  changeLabel(labels[3],1000-counter);
  changeLabel(labels[4],500 - counter);
  counter++;
  if (counter > 1000)
    counter = 0;
}


void changeLabel(GtkLabel *label, int num)
{
    char stri[10];
    str(stri, num);
    gtk_label_set_text(label,stri);
}

void changeLabeldbl(GtkLabel *label, double num)
{
  char stri[10];
  strdbl(stri,num);
  gtk_label_set_text(label,stri);
}

#include <gtk/gtk.h>

int counter = 0;

void on_Mainwindow_destroy()
{
    gtk_main_quit();
}

void on_quit_clicked()
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

  changeLabeldbl(labels[0],(rand() % 210)/10);
  changeLabeldbl(labels[1],(rand() % 50000));
  changeLabeldbl(labels[4],(rand()%300)/10);
  changeLabel(labels[6],(rand()%50));
  changeLabel(labels[7],(rand()%40));
  changeLabel(labels[8],(rand()%40));
  changeLabel(labels[9],(rand()%40));
  changeLabel(labels[10],(rand()%40));

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

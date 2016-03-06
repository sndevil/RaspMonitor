#include <gtk/gtk.h>

typedef enum { false, true } bool;
bool powertoggle = false;

void on_Mainwindow_destroy()
{
    gtk_main_quit();
}

void on_quit_clicked()
{
  gtk_main_quit();
}

void on_Powertoggle_toggled()
{
  powertoggle = (powertoggle == true) ? false : true;
  printf("Powertoggle toggled: %d\n",powertoggle);
}

void on_Onoffsetbtn_clicked()
{
  printf("toggled: %d\n",powertoggle);

}

void on_Sendpowersetbtn_clicked(GtkButton *button, gpointer user_data)
{
  double val;
  GtkAdjustment* scale = GTK_ADJUSTMENT(user_data);
  val = gtk_adjustment_get_value(scale);
  printf("value: %f\n",val);
}

void on_Sendratesetbtn_clicked(GtkButton *button, gpointer user_data)
{
  double val;
  GtkAdjustment* scale = GTK_ADJUSTMENT(user_data);
  val = gtk_adjustment_get_value(scale);
  printf("value: %f\n",val);
}

void on_Receiveratesetbtn_clicked(GtkButton *button, gpointer user_data)
{
  double val;
  GtkAdjustment* scale = GTK_ADJUSTMENT(user_data);
  val = gtk_adjustment_get_value(scale);
  printf("value: %f\n",val);
}

void on_Sendfreqsetbtn_clicked(GtkButton *button, gpointer user_data)
{
  double val;
  GtkAdjustment* scale = GTK_ADJUSTMENT(user_data);
  val = gtk_adjustment_get_value(scale);
  printf("value: %f\n",val);
}

void on_Receivefreqsetbtn_clicked(GtkButton *button, gpointer user_data)
{
  double val;
  GtkAdjustment* scale = GTK_ADJUSTMENT(user_data);
  val = gtk_adjustment_get_value(scale);
  printf("value: %f\n",val);
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
  changeLabel(labels[6],(int)(rand()%50));
  changeLabel(labels[7],(int)(rand()%40));
  changeLabel(labels[8],(int)(rand()%40));
  changeLabel(labels[9],(int)(rand()%40));
  changeLabel(labels[10],(int)(rand()%40));
  changeLabel(labels[12],(int)(rand()%40));
  changeLabel(labels[13],(int)(rand()%40));
  changeLabel(labels[14],(int)(rand()%40));
  changeLabel(labels[15],(int)(rand()%40));
  changeLabel(labels[16],(int)(rand()%40));
  changeLabel(labels[17],(int)(rand()%40));
  changeLabeldbl(labels[18],(rand()%24));
  changeLabeldbl(labels[19],(rand()%4));
  changeLabeldbl(labels[21],rand());
  changeLabeldbl(labels[23],rand());
  changeLabeldbl(labels[24],rand());
  changeLabeldbl(labels[25],rand());

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

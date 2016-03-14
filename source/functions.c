#include <gtk/gtk.h>

#define PROCESS_MEDIUM_THRESHOLD 20
#define CONTROL_MEDIUM_THRESHOLD 20
#define RACK_MEDIUM_THRESHOLD 20
#define PA_MEDIUM_THRESHOLD 20

#define PROCESS_HIGH_THRESHOLD 30
#define CONTROL_HIGH_THRESHOLD 30
#define RACK_HIGH_THRESHOLD 30
#define PA_HIGH_THRESHOLD 30

typedef enum { false, true } bool;
bool powertoggle = false;
bool datareceived = false;

enum lockstat { locked, searching, found};
//typedef enum { locked, notlocked} LockStat;

enum lockstat carrierstat = searching;
enum lockstat timingstat = searching;
enum lockstat systemstat = searching;
enum lockstat externalsourcestat = searching;

double processtemp=0,controltemp=0, racktemp=0, patemp=0;

void on_Mainwindow_destroy()
{
    gtk_main_quit();
}

void on_quit_clicked()
{
  gtk_main_quit();
}

void on_demotoggle_toggled()
{
  datareceived = (datareceived) ? false : true;
  printf("demo mode toggled: %d\n",datareceived);
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
  if (timingstat > 2)
    timingstat = 0;
  if (carrierstat > 2)
    carrierstat = 0;
  if (systemstat > 2)
    systemstat = 0;
  if (externalsourcestat > 2)
    externalsourcestat = 0;
  g_timeout_add_seconds(1,tick,labels);
  if (datareceived)
    {
      gtk_spinner_start(GTK_SPINNER(labels[28]));
      gtk_statusbar_push(GTK_STATUSBAR(labels[27]),1,"Receiving Data");
      processtemp = rand()%40;
      controltemp = rand()%40;
      racktemp = rand()%40;
      patemp = rand()%40;  
      changeLabeldbl(labels[0],(rand() % 210)/10);
      changeLabeldbl(labels[1],(rand() % 50000));
      changeStatusLabel(labels[2],carrierstat++);
      changeStatusLabel(labels[3],timingstat++);
      changeLabeldbl(labels[4],(rand()%300)/10);
      changeLabel(labels[6],(int)(rand()%50));
      changeLabel(labels[7],(int)(processtemp));
      changeLabel(labels[8],(int)(controltemp));
      changeLabel(labels[9],(int)(racktemp));
      changeLabel(labels[10],(int)(patemp));
      changeLabel(labels[12],(int)(rand()%40));
      changeLabel(labels[13],(int)(rand()%40));
      changeLabel(labels[14],(int)(rand()%40));
      changeLabel(labels[15],(int)(rand()%40));
      changeLabel(labels[16],(int)(rand()%40));
      changeLabel(labels[17],(int)(rand()%40));
      changeLabeldbl(labels[18],(rand()%24));
      changeLabeldbl(labels[19],(rand()%4));
      changeStatusLabel(labels[20],systemstat++);
      changeLabeldbl(labels[21],rand());
      changeStatusLabel(labels[22],externalsourcestat++);
      changeLabeldbl(labels[23],rand());
      changeLabeldbl(labels[24],rand());
      changeLabeldbl(labels[25],rand());
      checkTemperature(labels[26]);
    }
  else
    {
      gtk_statusbar_push(GTK_STATUSBAR(labels[27]),1,"No data");
      processtemp = 0;
      controltemp = 0;
      racktemp = 0;
      patemp = 0;
      gtk_spinner_stop(GTK_SPINNER(labels[28]));
      checkTemperature(labels[26]);
    }
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

void changeStatusLabel(GtkLabel *label, enum lockstat status)
{
  switch (status)
    {
    case locked:
      gtk_label_set_text(label,"Locked");
      break;
    case searching:
      gtk_label_set_text(label,"Searching");
      break;
    case found:
      gtk_label_set_text(label,"Found");
      break;
    }
}

void checkTemperature(GtkLabel *label)
{
  if (processtemp > PROCESS_HIGH_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ff9999\" size=\"medium\">Process Temperature ALERT</span>");
  else if (controltemp > CONTROL_HIGH_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ff9999\" size=\"medium\">Control Temperature ALERT</span>");
  else if (racktemp > RACK_HIGH_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ff9999\" size=\"medium\">Rack Temperature ALERT</span>");
  else if (patemp > PA_HIGH_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ff9999\" size=\"medium\">PA Temperature ALERT</span>");
  else if (processtemp > PROCESS_MEDIUM_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ffff99\" size=\"medium\">Process Temperature High</span>");
  else if (controltemp > CONTROL_MEDIUM_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ffff99\" size=\"medium\">Control Temperature High</span>");
  else if (racktemp > RACK_MEDIUM_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ffff99\" size=\"medium\">Rack Temperature High</span>");
  else if (patemp > PA_MEDIUM_THRESHOLD)
      gtk_label_set_markup(label,"<span background=\"#ffff99\" size=\"medium\">PA Temperature High</span>");
  else
    gtk_label_set_text(label, "");
}

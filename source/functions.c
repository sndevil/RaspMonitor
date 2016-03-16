#include <gtk/gtk.h>
#include <math.h>
#include <time.h>

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
  double rand1,rand2,rand3,rand4;

  g_timeout_add_seconds(1,tick,labels);  

  rand1 = (double)(rand()%4000)/100;
  rand2 = (double)(rand()%10000)/100;
  rand3 = (double)(rand()%100000)/1000000;
  rand4 = (double)(rand()%2400)/100;
  if (timingstat > 2)
    timingstat = 0;
  if (carrierstat > 2)
    carrierstat = 0;
  if (systemstat > 2)
    systemstat = 0;
  if (externalsourcestat > 2)
    externalsourcestat = 0;


  if (datareceived)
    {
      writelog(labels[30],"data received");
      gtk_spinner_start(GTK_SPINNER(labels[28]));
      gtk_statusbar_push(GTK_STATUSBAR(labels[27]),1,"Receiving Data");
      processtemp = rand1;
      controltemp = rand1;
      racktemp = rand1;
      patemp = rand1;  
      changeLabeldbl(labels[0],(double)(rand() % 210)/10);
      changeLabeldbl(labels[1],(rand() % 50000));
      changeStatusLabel(labels[2],carrierstat++);
      changeStatusLabel(labels[3],timingstat++);
      changeLabeldbl(labels[4],(double)(rand()%300)/10);
      changelabelBER(labels[5],rand3);
      changeLabel(labels[6],(int)(rand()%50));
      changeLabel(labels[7],(int)(processtemp));
      changeLabel(labels[8],(int)(controltemp));
      changeLabel(labels[9],(int)(racktemp));
      changeLabel(labels[10],(int)(patemp));
      changeLabel(labels[12],(int)(rand1));
      changeLabel(labels[13],(int)(rand1));
      changeLabel(labels[14],(int)(rand1));
      changeLabel(labels[15],(int)(rand1));
      changeLabel(labels[16],(int)(rand1));
      changeLabel(labels[17],(int)(rand1));
      changeLabeldbl(labels[18],(rand4));
      changeLabeldbl(labels[19],(rand1));
      changeStatusLabel(labels[20],systemstat++);
      changeLabeldbl(labels[21],rand2);
      changeStatusLabel(labels[22],externalsourcestat++);
      changeLabeldbl(labels[23],rand2);
      changeLabeldbl(labels[24],rand2);
      changeLabeldbl(labels[25],rand2);
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

void changelabelBER(GtkLabel *label, double value)
{
  int power;
  double base;
  char out[13];
  power = floor(log10(value));
  base = value / (pow(10,power));
  snprintf(out,13,"%e",value);
  gtk_label_set_text(label,out);
}

void writelog(GtkTextBuffer *label, const char* str)
{
  gchar* out;//,current = "hello";
  char current[20];
  time_t seconds,minute,hour,timet;
  GtkTextIter textiter;

  timet = time(NULL);
  seconds = timet%60;
  minute = (timet/60)%60;
  hour = (timet/3600)%24;
  snprintf(current,strlen(str)+13,"%02d:%02d:%02d : %s\n",hour,minute,seconds,str);
  gtk_text_buffer_get_start_iter(label,&textiter);
  gtk_text_buffer_insert(label,&textiter,current,strlen(current));

}

void clear_log(GtkTextBuffer *label)
{
  gtk_label_set_text(label,"");
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

#include <gtk/gtk.h>
#include <math.h>
#include <time.h>
#include <termios.h>

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

bool portopen = false;

enum lockstat { locked, searching, found};

enum lockstat carrierstat = searching;
enum lockstat timingstat = searching;
enum lockstat systemstat = searching;
enum lockstat externalsourcestat = searching;

double processtemp=0,controltemp=0, racktemp=0, patemp=0;
double rand1,rand2,rand3,rand4;
int USB;

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

void on_Clearbtn_clicked(GtkButton *button, gpointer user_data)
{
  GtkTextIter start,end;
  GtkTextBuffer *buffer = user_data;// GTK_TEXTBUFFER(user_data);
  gtk_text_buffer_get_start_iter(buffer,&start);
  gtk_text_buffer_get_end_iter(buffer,&end);
  gtk_text_buffer_delete(buffer,&start,&end);
}

void on_openportbtn_clicked(GtkButton *button, gpointer user_data)
{
  USB = SerialOpen("/dev/ttyUSB0",(speed_t)B4800);
  portopen = true;
  printf("Port Opened: %d\n",USB);
}


void on_Savebtn_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *dialog;
  GtkTextBuffer *textbuffer;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  textbuffer = user_data;
  dialog = gtk_file_chooser_dialog_new ("Save File",
                                      NULL,
                                      action,
                                      "_Cancel",
                                      GTK_RESPONSE_CANCEL,
                                      "_Save",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);
  chooser = GTK_FILE_CHOOSER (dialog);

  gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_current_name (chooser,
                                       "Untitled document.txt");

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      char *content;
      GtkTextIter start,end;
      FILE *f;

      filename = gtk_file_chooser_get_filename (chooser);
      gtk_text_buffer_get_start_iter(textbuffer,&start);
      gtk_text_buffer_get_end_iter(textbuffer,&end);
      content = gtk_text_buffer_get_text(textbuffer,&start,&end,FALSE);
      
      f = fopen(filename,"w");
      fwrite(content,sizeof(char),strlen(content),f);
      fclose(f);
    }

 gtk_widget_destroy (dialog);
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
  double rand5,rand6,rand7;
  rand1 = (double)(rand()%4000)/100;
  rand5 = (double)(rand()%4000)/100;
  rand6 = (double)(rand()%4000)/100;
  rand7 = (double)(rand()%4000)/100;
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

  if (portopen == true)
    {
      char* input = SerialRead(USB);
      printf("input data: %s  length: %d\n",input,strlen(input));
    }

  if (datareceived)
    {
      gtk_spinner_start(GTK_SPINNER(labels[28]));
      gtk_statusbar_push(GTK_STATUSBAR(labels[27]),1,"Receiving Data");
      processtemp = rand1;
      controltemp = rand5;
      racktemp = rand6;
      patemp = rand7;  
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
      checkTemperature(labels[30]);
    }
  else
    {
      gtk_statusbar_push(GTK_STATUSBAR(labels[27]),1,"No data");
      processtemp = 0;
      controltemp = 0;
      racktemp = 0;
      patemp = 0;
      gtk_spinner_stop(GTK_SPINNER(labels[28]));
      checkTemperature(labels[30]);
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
  gchar* out;
  char current[40];
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

void checkTemperature(GtkTextBuffer *label)
{
  if (processtemp > PROCESS_HIGH_THRESHOLD)
      writelog(label,"Process Temperature ALERT");
  if (controltemp > CONTROL_HIGH_THRESHOLD)
      writelog(label,"Control Temperature ALERT");
  if (racktemp > RACK_HIGH_THRESHOLD)
      writelog(label,"Rack Temperature ALERT");
  if (patemp > PA_HIGH_THRESHOLD)
    writelog(label,"PA Temperature ALERT");
  if (processtemp > PROCESS_MEDIUM_THRESHOLD)
    writelog(label,"Process Temperature High");
  if (controltemp > CONTROL_MEDIUM_THRESHOLD)
    writelog(label,"Control Temperature High");
  if (racktemp > RACK_MEDIUM_THRESHOLD)
    writelog(label,"Rack Temperature High");
  if (patemp > PA_MEDIUM_THRESHOLD)
    writelog(label,"PA Temperature High");

}

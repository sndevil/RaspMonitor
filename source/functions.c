#include <gtk/gtk.h>
#include <math.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>

#define PROCESS_MEDIUM_THRESHOLD 20
#define CONTROL_MEDIUM_THRESHOLD 20
#define RACK_MEDIUM_THRESHOLD 20
#define PA_MEDIUM_THRESHOLD 20

#define PROCESS_HIGH_THRESHOLD 30
#define CONTROL_HIGH_THRESHOLD 30
#define RACK_HIGH_THRESHOLD 30
#define PA_HIGH_THRESHOLD 30

#define SERIAL_BUFFER_SIZE 8192

const char* KeyboardPath = "/dev/ttyUSB0";
const char* Serial1Path = "/dev/ttyUSB1";

const char* AlertsPath = "alerts/";

typedef enum { false, true } bool;
bool powertoggle = false;
bool datareceived = false;

bool portopen = false;
bool keyboardopen = false;
bool typing = false;
double typenum = 0;

enum lockstat { locked, searching, found};

enum lockstat carrierstat = searching;
enum lockstat timingstat = searching;
enum lockstat systemstat = searching;
enum lockstat externalsourcestat = searching;

double processtemp=0,controltemp=0, racktemp=0, patemp=0;
double rand1,rand2,rand3,rand4;
int focusnum = 0,idx = 0;
int btnfocusnum = 0;
int alarmfocusnum = 0;

int afterdot = 0;
bool dotpressed = false;
double set[6] = {1,35,1024,1024,2,1.5};
int USB,KEYBOARD;

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
  if (portopen == false)
    {
      USB = SerialOpen(Serial1Path,(speed_t)B115200);
      portopen = true;
      printf("Port Opened: %d\n",USB);
      gtk_button_set_label(button, "Close Port");
    }
  else
    {
      close(USB);
      portopen = false;
      printf("Port%d Closed\n", USB);
      gtk_button_set_label(button, "Open Port");
    }
}

void on_sendbtn_clicked(GtkButton *button, gpointer user_data)
{
  char tosend[7] = {'$','C','M','D','\0','C','\t'};
  //printf("sending data %s, length:%d\n",tosend,strlen(tosend));
  SerialWrite(tosend,USB);
}


void on_Savebtn_clicked(GtkButton *button, gpointer user_data)
{
  //GtkWidget *dialog;
  GtkTextBuffer *textbuffer;
  //GtkFileChooser *chooser;
  //GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  textbuffer = user_data;
  /*dialog = gtk_file_chooser_dialog_new ("Save File",
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
  {*/
      char filename[17];
      char filepath[30];
      char *content;
      GtkTextIter start,end;
      FILE *f;
      time_t second,minute,hour,timet;
      timet = time(NULL);
      second = timet%60;
      minute = (timet/60)%60;
      hour = (timet/3600)%60;

      printf("starting parse\n");
      snprintf(filename,17,"File%02d-%02d-%02d.txt",hour,minute,second);
      printf("Filename: %s\n",filename);
      snprintf(filepath,strlen(filename) + strlen(AlertsPath)+1,"%s%s",AlertsPath,filename);

      printf("Filepath: %s\n",filepath);
      //filename = gtk_file_chooser_get_filename (chooser);
      gtk_text_buffer_get_start_iter(textbuffer,&start);
      gtk_text_buffer_get_end_iter(textbuffer,&end);
      content = gtk_text_buffer_get_text(textbuffer,&start,&end,FALSE);
      
      f = fopen(filepath,"w");
      fwrite(content,sizeof(char),strlen(content),f);
      fclose(f);
      //  }

      // gtk_widget_destroy (dialog);
}

void on_Loadbtn_clicked(GtkButton *button, gpointer user_data)
{
  
}

void str(char* out, int i)
{
  snprintf(out,10,"%d",i);
}

void strdbl(char* out, double i)
{
  snprintf(out,15,"%f",i);
 }

void tick(GtkLabel** labels)
{
  double rand5,rand6,rand7;
  int i,pps=0;
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
      char input[SERIAL_BUFFER_SIZE];
      memset(input,'\r',sizeof input);
      SerialRead(USB,&input);
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

void keyboardtick(GtkLabel** labels)
{ 
  //keyboardopen = true;//comment this
  if (keyboardopen)
    {
      //   $K[CODE]E!
      //   [CODE] is 1 byte
      //   [CODE] default = 0xFF
      int bytestoread = -1;
      char input[5] = "$K1E!";
      memset(input,'\r',sizeof input);
      ioctl(KEYBOARD, FIONREAD, &bytestoread);
      if (bytestoread >= 5)
	SerialRead(KEYBOARD,&input,5);
      printf(".");
      
      if (input[0] == '$' && input[1] == 'K' && input[4] == '!')
	{
	  int command = (int)input[2];
	  int pagenum = gtk_notebook_get_current_page(GTK_NOTEBOOK(labels[34]));
	  gdouble value = 0;
	  double typed;
	  GtkAdjustment * adjust;
	  GtkWidget * temp;
	  switch (command)
	    {
	    case 1: //Up Arrow
	      switch (pagenum)
		{
		case 0:
		  adjust = gtk_scrolled_window_get_vadjustment(labels[31]);
		  value = gtk_adjustment_get_value(adjust);
		  gtk_adjustment_set_value(adjust,(value>10) ? value - 10:0);
		  printf(".");		 
		  break;
		case 1:
		  typing = false;
		  dotpressed = false;
		  afterdot = 0;
		  temp = labels[35 + 2*focusnum];
		  changeentry(temp,set[focusnum]);
		  if (--focusnum <0)
		    focusnum = 5;
		  temp = labels[35 + 2*focusnum];
		  gtk_widget_grab_focus(temp);
		  //gtk_widget_grab_focus(labels[53]);
		  printf(".");
		  break;
		case 2:
		  if (--alarmfocusnum < 0)
		    alarmfocusnum = 1;
		  temp = labels[54+alarmfocusnum];
		  gtk_test_widget_send_key(temp,GDK_KEY_Return,0);
		  printf(".");
		  break;
		case 3:
		  if (--btnfocusnum < 0)
		    btnfocusnum = 5;
		  gtk_test_widget_send_key(labels[47+btnfocusnum],GDK_KEY_Return,0);
		  printf(".");
		  break;
		}
	      break;
	    case 2: //Down Arrow
	      switch (pagenum)
		{
		case 0:
		  adjust = gtk_scrolled_window_get_vadjustment(labels[31]);
		  value = gtk_adjustment_get_value(adjust);
		  gtk_adjustment_set_value(adjust,(value<250) ? value + 10:260);
		  printf(".");		 
		  break;
		case 1:
		  typing = false;
		  dotpressed = false;
		  afterdot = 0;
		  temp = labels[35 + 2*focusnum];
		  changeentry(temp,set[focusnum]);		  
		  if (++focusnum > 5)
		    focusnum = 0;
		  temp = labels[35 + 2*focusnum];
		  gtk_widget_grab_focus(temp);
		  printf(".");
		  break;
		case 2:
		  if (++alarmfocusnum > 1)
		    alarmfocusnum = 0;
		  temp = labels[54+alarmfocusnum];
		  gtk_test_widget_send_key(temp,GDK_KEY_Return,0);
		  printf(",");
		  break;
		case 3:
		  if (++btnfocusnum > 5)
		    btnfocusnum = 0;
		  gtk_test_widget_send_key(labels[47+btnfocusnum],GDK_KEY_Return,0);
		  break;
		}	    
	      break;
	    case 3: //Tab Button
	      if (pagenum == 3)
		gtk_notebook_set_current_page(GTK_NOTEBOOK(labels[34]),0);
	      else
	      {
		gtk_notebook_next_page(GTK_NOTEBOOK(labels[34]));
		if (pagenum == 0)
		  {
		    changeentry(labels[35],set[0]);
		    changeentry(labels[37],set[1]);
		    changeentry(labels[39],set[2]);
		    changeentry(labels[41],set[3]);
		    changeentry(labels[43],set[4]);
		    changeentry(labels[45],set[5]);
		    gtk_widget_grab_focus(labels[35+2*focusnum]);
		  }
		else if (pagenum ==1)
		  gtk_test_widget_send_key(labels[54],GDK_KEY_Tab,0);
		else if (pagenum ==2)
		  {
		    btnfocusnum = 0;
		    gtk_test_widget_send_key(labels[47],GDK_KEY_Tab,0);
		  }
     	      }
	      printf(".");
	      break;
	    case 4: //Enter Button
	      if (pagenum == 1)
		{
	      gettyped(&typed);
	      switch (focusnum)
		{
		case 0:
		  if (typed == 1)
		    {
		      gtk_label_set_text(labels[11],"On");
		      printf("Powering On\n");		      
		      set[0] = 1;
		    }
		  else if (typed == 0)
		    {
		      gtk_label_set_text(labels[11],"Off");
		      printf("Powering Off\n");		      
		      set[0] = 0;
		    }
		  else
		    {
		      printf ("Not a valid input\n");
		      changeentry(labels[35+2*focusnum],set[0]);
		      typing = false;
		      dotpressed = false;
		      afterdot = 0;
		    }
		  break;
		case 1:
		  if (typed <= 43 && typed >= 30)
		    {		      
		      changeLabeldbl(labels[12],typed);
		      printf("Setting SendPower to %f\n",typed);
		      set[1] = typed;
		    }
		  else
		    {
		      printf("Not a valid input\n");
		      changeentry(labels[35+2*focusnum],set[1]);
		      typing = false;
		      dotpressed = false;
		      afterdot = 0;
		    }
		  break;
		case 2:
		  if (typed >= 1 && typed <= 2048)
		    {		      
		      changeLabeldbl(labels[13],typed);
		      printf("Setting Sentrate to %f\n",typed);
		      set[2] = typed;
		    }
		  else
		    {
		      printf("Not a valid input\n");
		      changeentry(labels[35+2*focusnum],set[2]);
		      typing = false;
		      dotpressed = false;
		      afterdot = 0;
		    }			    
		  break;
		case 3:
		  if (typed >= 1 && typed <= 2048)
		    {		      
		      changeLabeldbl(labels[14],typed);
		      printf("Setting Receiverate to %f\n",typed);
		      set[3] = typed;
		    }
		  else
		    {
		      printf("Not a valid input\n");
		      changeentry(labels[35+2*focusnum],set[3]);
		      typing = false;
		      dotpressed = false;
		      afterdot = 0;
		    }		    
		  break;
		case 4:
		  if (typed >= 0.1 && typed <= 3)
		    {		      
		      changeLabeldbl(labels[15],typed);
		      printf("Setting SendFreq to %f\n",typed);
		      set[4] = typed;
		    }
		  else
		    {
		      printf("Not a valid input\n");
		      changeentry(labels[35+2*focusnum],set[4]);
		      typing = false;
		      dotpressed = false;
		      afterdot = 0;
		    }
		  break;
		case 5:
		  if (typed >= 0.1 && typed <= 3)
		    {
		      changeLabeldbl(labels[16],typed);
		      printf("Setting ReceiveFreq to %f\n",typed);
		      set[5] = typed;
		    }
		  else
		    {
		      printf("Not a valid input\n");
		      changeentry(labels[35+2*focusnum],set[5]);
		      typing = false;
		      dotpressed = false;
		      afterdot = 0;				  
		    }
		  break;
		}
		}
	      else if (pagenum == 2)
		{
		  if (alarmfocusnum == 0)
		    on_Savebtn_clicked(labels[54],labels[56]);
		  else if (alarmfocusnum == 1)
		    on_Clearbtn_clicked(labels[55],labels[56]);

		  printf(",");
		}
	      else if (pagenum == 3)
		{
		  switch (btnfocusnum)
		    {
		    case 0:
		      on_openportbtn_clicked(labels[47],(gpointer)0);
		      printf(".");
		      break;
		    case 1:
		      on_sendbtn_clicked(labels[48],(gpointer)0);
		      break;
		    case 2:
		      on_demotoggle_toggled();
		      break;
		    case 3:
		      on_Savebtn_clicked(labels[50],(gpointer)0);
		      break;
		    case 4:
		      on_Loadbtn_clicked(labels[51],(gpointer)0);
		      break;
		    case 5:
		      on_quit_clicked(labels[52],(gpointer)0);
		      break;
		    }
		}
	      break;
	    case ((int)'1'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);
		  numberentered(temp,1);
		}
	      printf(".");
	      break;
	    case ((int)'2'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);
		  numberentered(temp,2);
		}
	      printf(".");
	      break;
	    case ((int)'3'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);
		  numberentered(temp,3);
		}
	      printf(".");	      
	      break;
	    case ((int)'4'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);	      
		  numberentered(temp,4);
		}
	      printf(".");	      
	      break;
	    case ((int)'5'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);	      
		  numberentered(temp,5);
		}
	      printf(".");	      
	      break;
	    case ((int)'6'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);	      
		  numberentered(temp,6);
		}
	      printf(".");	      
	      break;
	    case ((int)'7'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);	      
		  numberentered(temp,7);
		}
	      printf(".");	      
	      break;
	    case ((int)'8'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);	      
		  numberentered(temp,8);
		}
	      printf(".");	      
	      break;
	    case ((int)'9'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);
		  numberentered(temp,9);
		}
	      printf(".");	      
	      break;
	    case ((int)'0'):
	      if (pagenum == 1)
		{
		  temp = GTK_ENTRY(labels[35 + 2*focusnum]);
		  numberentered(temp,0);
		}
	      printf(".");	      
	      break;
	    case ((int)'.'):
	      if (pagenum == 1)
		{
		  dotpressed = true;
		  afterdot = 0;
		}
	      printf(".");	      
	      break;
	    }
	}

    }
  else
    {
      KEYBOARD = SerialOpen(KeyboardPath ,(speed_t)B115200);
      printf("Keyboard Opened : %d\n",KEYBOARD);
      if (KEYBOARD >= 0)
	keyboardopen = true;
      else	
	printf("Error opening keyboard\n");
    }
}

void numberentered(GtkEntry * entry, int input)
{
  double output = 0;
  if (typing)
  typenum = typenum* 10 + input;
  else
   {
     typenum = input;
     typing = true;
   }
  if (dotpressed)
     afterdot++;
  gettyped(&output);
  changeentry(entry,output);
}

void gettyped(double* out)
{
  int i;
  (*out) = typenum;
  for (i = 0 ; i < afterdot; i++)
    (*out) /= 10;
}

void changeentry(GtkEntry *entry, double num)
{
  char stri[15];
  strdbl(stri,num);
  gtk_entry_set_text(entry,stri);
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



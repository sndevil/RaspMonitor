void on_Mainwindow_destroy();
void on_quit_clicked();
void on_demotoggle_toggled();
void on_Powertoggle_toggled();
void on_Onoffsetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Sendpowersetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Sendratesetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Receiveratesetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Sendfreqsetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Receivefreqsetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Clearbtn_clicked(GtkButton *button, gpointer user_data);
void on_Savebtn_clicked(GtkButton *button, gpointer user_data);
void on_Loadbtn_clicked(GtkButton *button, gpointer user_data);

void tick(GtkLabel** labels);
void keyboardtick(GtkLabel** labels);
void str(char* out,int i);
void strdbl(char* out,double i);
void gettyped(double* out);
void numberentered(GtkEntry *entry, int input);
void changeentry(GtkEntry *entry, double num);
void changeLabel(GtkLabel *label, int num);
void changeLabeldbl(GtkLabel *label, double num);
void changeStatusLabel(GtkLabel *label, enum lockstat status);
void changelabelBER(GtkLabel *label, double value);
void writelog(GtkTextBuffer *label, const char* str);
void clear_log(GtkTextBuffer *label);

void checkTemperature(GtkTextBuffer *label);

void on_Mainwindow_destroy();
void on_quit_clicked();
void on_Powertoggle_toggled();
void on_Onoffsetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Sendpowersetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Sendratesetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Receiveratesetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Sendfreqsetbtn_clicked(GtkButton *button,gpointer   user_data);
void on_Receivefreqsetbtn_clicked(GtkButton *button,gpointer   user_data);
void tick(GtkLabel** labels);
void str(char* out,int i);
void strdbl(char* out,double i);
void changeLabel(GtkLabel *label, int num);
void changeLabeldbl(GtkLabel *label, double num);

void checkTemperature(GtkLabel *label);

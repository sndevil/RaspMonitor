#include <gtk/gtk.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>

#define INPUT_BUFFER_SIZE 8192

typedef enum { false, true } bool;

bool status = false;
int changed = 0;

int SerialOpen(char* devicename,speed_t Baudrate)
{
  int USB;
  struct termios tty;
  struct termios tty_old;
  memset (&tty, 0, sizeof tty);
  USB = open(devicename, O_RDWR| O_NOCTTY );
  /* Error Handling */
  if ( tcgetattr ( USB, &tty ) != 0 ) {
    printf("Error %d from tcgetattr: %s \n", errno, strerror(errno));
  }

  /* Save old tty parameters */
  tty_old = tty;

  /* Set Baud Rate */
  cfsetospeed (&tty, Baudrate);
  cfsetispeed (&tty, Baudrate);

  /* Setting other Port Stuff */
  tty.c_cflag     &=  ~PARENB;            // Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;

  tty.c_cflag     &=  ~CRTSCTS;           // no flow control
  tty.c_cc[VMIN]   =  1;                  // read doesn't block
  tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
  tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

  /* Make raw */
  cfmakeraw(&tty);

  /* Flush Port, then applies attributes */
  tcflush( USB, TCIFLUSH );
  if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
    printf("Error %d from tcsetattr\n",errno);
  }
  return USB;
}

void SerialWrite(char* towrite, int Device)
{
  int n_written = 0,
    spot = 0,i=0;
  //for (i = 0; i < strlen(towrite);i++)
  //  printf("Starting to write %c\n",towrite[i]);
  do {
    n_written = write( Device, &towrite[spot], 1 );
    spot += n_written;
    //printf("written %d bytes: %d\n", n_written,towrite[spot-1]);
  } while (spot < 7 && n_written > 0);
}

void SerialRead(int Device,char* buffer,int len)
{
  int n = 0,
    spot = 0, pps = 0;
  char buf;
  time_t starttime;
  starttime = time(NULL);

  do {
    n = read( Device, &buf, 1 );
    if (buf == 126)
      pps++;
    sprintf( &buffer[spot], "%c", buf );
    spot += n;
  } while( spot< len && n > 0 && starttime == time(NULL));

  if (n < 0) {
    printf("Error reading: %s\n",strerror(errno));
    return;
  }
  else if (n == 0) {
    printf("Read nothing!\n");
  }
}

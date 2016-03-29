#include <fcntl.h>
#include <errno.h>
#include <termios.h>

char response[2048];
int SerialOpen(char* devicename,speed_t Baudrate)
{
  int USB;
  struct termios tty;
  struct termios tty_old;
  memset (&tty, 0, sizeof tty);
  memset(response, '\0', sizeof response);
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
  //unsigned char cmd[] = "INIT \r";
  int n_written = 0,
    spot = 0;

  do {
    n_written = write( Device, &towrite[spot], 1 );
    spot += n_written;
  } while (towrite[spot-1] != '\r' && n_written > 0);
}

void SerialRead(int Device,char* buffer)
{
  int n = 0,
    spot = 0,i;
  char buf;
  char res[2048];
  do {
    n = read( Device, &buf, 1 );
    sprintf( &res[spot], "%c", buf );
    spot += n;
  } while( spot<2048 && n > 0);

  for (i = 0; i < spot; i++)
      buffer[i] = res[i];

  if (n < 0) {
    printf("Error reading: %s\n",strerror(errno));
    return;
  }
  else if (n == 0) {
    printf("Read nothing!\n");
  }
}

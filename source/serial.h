
int SerialOpen(char* devicename, speed_t Baudrate);
void SerialWrite(char* towrite, int Device);
void SerialRead(int Device,char* buffer);

int SerialOpen(char* devicename, speed_t Baudrate);
void SerialWrite(char* towrite, int Device);
char* SerialRead(int Device);

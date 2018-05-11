
typedef unsigned long DWORD; 

int COMOpen();
void COMClose();
unsigned char* ReadThread();
void WriteThread();
int Read(char* buf,DWORD len);

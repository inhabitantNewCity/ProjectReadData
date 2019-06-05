#include "stdafx.h"

#include <Windows.h>
#include <io.h>
#include "com_port.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

#define BUFSIZE 28
unsigned char bufrd[BUFSIZE];	//приёмный буфер
HANDLE COMport;					//дескриптор порта
OVERLAPPED overlapped;			//для  чтения 
OVERLAPPED overlappedwr;		//для  записи
unsigned long counter;			//счётчик принятых байтов, обнуляется при каждом открытии порта
bool Connected = true; 
/*
ERROR
1  - ok
-1 - ошибка открытия COMport'a
-2 - oшибка применения настроек

*/
int COMOpen()
{
 DCB dcb;               //структура для общей инициализации порта DCB
 
 COMport = CreateFile(L"\\\\.\\\COM4",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
		//FILE_FLAG_OVERLAPPED,
        FILE_ATTRIBUTE_NORMAL,
        0);
 if (COMport == INVALID_HANDLE_VALUE) 
	{	
		//MessageBox(0,TEXT("Cannot open the port"),TEXT("Error"),MB_ICONEXCLAMATION);
		Connected = FALSE;
		return -1;
	}
 else
 { 
		dcb.BaudRate = CBR_115200;
		dcb.fBinary = true;     /* Binary Mode (skip EOF check)    */
		dcb.fParity = false;     /* Enable parity checking          */
		dcb.fOutxCtsFlow = false; /* CTS handshaking on output       */
		dcb.fOutxDsrFlow = false; /* DSR handshaking on output       */
		dcb.fDtrControl = DTR_CONTROL_ENABLE;  /* DTR Flow control    */
		dcb.fDsrSensitivity = false; /* DSR Sensitivity              */
		dcb.fTXContinueOnXoff = true; /* Continue TX when Xoff sent */
		dcb.fOutX = false;       /* Enable output X-ON/X-OFF        */
		dcb.fInX = false;        /* Enable input X-ON/X-OFF         */
		dcb.fErrorChar = 0;  /* Enable Err Replacement          */
		dcb.fNull = false;       /* Enable Null stripping           */
		dcb.fRtsControl = RTS_CONTROL_DISABLE;  /* Rts Flow control       */
		dcb.fAbortOnError = true; // false; /* Abort all reads and writes on Error */
		dcb.ByteSize = 8;        /* Number of bits/byte, 4-8        */
		dcb.Parity = NOPARITY;          /* 0-4=None,Odd,Even,Mark,Space    */
		dcb.StopBits = ONESTOPBIT;        /* 0,1,2 = 1, 1.5, 2               */

		COMMTIMEOUTS ComTimeouts;
		Connected =(bool) GetCommTimeouts (COMport, &ComTimeouts);
		if (!Connected)
			{
			// dwError = GetLastError();
			//MessageBox(0,TEXT("Cannot get timeouts"),TEXT("Error"),MB_ICONEXCLAMATION);
			}
        ComTimeouts.ReadIntervalTimeout = 0;
        ComTimeouts.ReadTotalTimeoutConstant = 100;
        ComTimeouts.ReadTotalTimeoutMultiplier = 1;
        ComTimeouts.WriteTotalTimeoutConstant = 100;
        ComTimeouts.WriteTotalTimeoutMultiplier = 1;

            if(!SetCommTimeouts(COMport, &ComTimeouts))
            {
                //cout << "Cann't SetCommTimeouts!\n";
                return 0;
            }
           // cout << "SetCommTimeouts\n";

 }
		if (SetCommState(COMport, &dcb))
		{
		return 1;		
		}
		else
			return -2;
}

void COMClose()
{  
 CloseHandle(COMport);          //закрыть порт
 COMport=0;						//обнулить переменную для дескриптора порта
}

//функция реализует приём байтов из COM-порта
 unsigned char* ReadThread()
{
 COMSTAT comstat;		//структура текущего состояния порта, в данной программе используется для определения количества принятых в порт байтов
 DWORD btr, temp,mask, signal;	//переменная temp используется в качестве заглушки
 overlapped.hEvent = CreateEvent(NULL, true, true, NULL);	//создать сигнальный объект-событие для асинхронных операций
  SetCommMask(COMport, EV_RXCHAR);                   	        //установить маску на срабатывание по событию приёма байта в порт
 while(1)						//пока поток не будет прерван, выполняем цикл
  {
   //cout<<"COM0 "<<endl;
   WaitCommEvent(COMport, &mask, &overlapped);               	//ожидать события приёма байта (это и есть перекрываемая операция)
   //cout<<"COM1 "<<endl;
   signal = WaitForSingleObject(overlapped.hEvent, INFINITE);	//приостановить поток до прихода байта
    //cout<<"COM2 "<<endl;
   if(signal == WAIT_OBJECT_0)				        //если событие прихода байта произошло
    {
     if(GetOverlappedResult(COMport, &overlapped, &temp, true)) //проверяем, успешно ли завершилась перекрываемая операция WaitCommEvent
      if((mask & EV_RXCHAR)!=0)					//если произошло именно событие прихода байта
       {
        ClearCommError(COMport, &temp, &comstat);		//нужно заполнить структуру COMSTAT
        btr = comstat.cbInQue;                          	//и получить из неё количество принятых байтов
        if(btr)                         			//если действительно есть байты для чтения
        {
         ReadFile(COMport, bufrd, btr, &temp, &overlapped);     //прочитать байты из порта в буфер программы
         counter+=btr;  //увеличиваем счётчик байтов	
         return bufrd; 		//вызываем функцию для вывода данных на экран и в файл
        }
       }
    }
  }
 CloseHandle(overlapped.hEvent);		//перед выходом из потока закрыть объект-событие
}

void WriteThread()
{
 DWORD temp;
 BYTE b=0;
 b='0';
 overlappedwr.hEvent = CreateEvent(NULL, true, true, NULL);   	 
 WriteFile(COMport, &b, 1, &temp,&overlappedwr); // передать 0 в порт
 //cout<<"WriteFile "<<temp<<endl;
 CloseHandle(overlappedwr.hEvent);
}
//-----------------------------------------------------------------------
int Read(char* buf,DWORD len)
{
 if(!Connected)
        return 0;

 DWORD read=1;
 BYTE a=0;
 BYTE b=0;

 memset(buf,0,sizeof(BYTE)*len);
 b='0';
 WriteFile(COMport,&b,1,&read,0);

 b=0;
 memset(buf,0,sizeof(BYTE)*len);
 while(read>0)
 {
	b=0;
	ReadFile(COMport,&b,1,&read,0);
	if(b!=0xFF)
		continue;
	buf[0]=b;

	b=0;
	ReadFile(COMport,&b,1,&read,0);
	if(b!=0xFF)
		continue;
	buf[1]=b;

	b=0;
	ReadFile(COMport,&b,1,&read,0);
	if(b!=0xFF)
		continue;
	buf[2]=b;

	b=0;
	ReadFile(COMport,&b,1,&read,0);
	if(b!=0xFF)
		continue;
	buf[3]=b;

	b=0;
	break;
	}

    if(read==0)
	 return 0;

    for(DWORD i=4;i<len;i++)
	{
	 ReadFile(COMport,&b,1,&read,0);

	 if(read==0)
	 {
		read=i;
		break;
	 }
	 else
	 {
		read=i+1;
		buf[i]=b;
	 }
	}
 return read;
}
//-----------------------------------------------------------------------
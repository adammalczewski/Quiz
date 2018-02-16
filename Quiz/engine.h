#ifndef ENGINE_H
#define ENGINE_H 

#include <windows.h>
#include "Questions.h"

extern bool COUT;
extern int coutn;

class TStream{
public:
  TStream operator<<(char* x){
	if (COUT){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD X;
    WriteConsole(hOut,x,strlen(x),&X,NULL);     
    }   
  }  
  TStream operator<<(int x);
  TStream operator<<(RECT rc);
};


extern TStream cout;

void CoutOn();
void CoutOff();
void ShowMessage(char* msg);
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);
void WriteInConsole(char* x);
int strtoint(char* x);
SStr inttostr(int x);
int Max(int x,int y);
double Maxd(double x,double y);
int Min(int x,int y);
double Mind(double x,double y);
RECT EmptyRect();
int PrintMemoryInfo( DWORD processID );
int MemoryUsage(HWND hwnd);
int FindFiles(const char *x);
char* OpenFile(HWND hwnd);

#endif

#include <windows.h>

class TStream{
public:
  TStream operator<<(char* x){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD X;
    WriteConsole(hOut,x,strlen(x),&X,NULL);        
  }        
};

extern TStream cout;

void ShowMessage(char* msg);
char* inttostr(int x);
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);
void WriteInConsole(char* x);
int strtoint(char* x);

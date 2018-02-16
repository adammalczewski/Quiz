#include <windows.h>

void ShowMessage(char* msg){
  MessageBox(NULL,msg,"Message",MB_OK);   
}

char* inttostr(int x){
  char* res = new char[10];
  if (x == 0){
    res[0] = '0';
    res[1] = 0;
    return res;
  }
  int i =9;
  while (x != 0){
    res[i] = 48+(x%10);
    x /= 10;
    i--;    
  }   
  for (int j = i+1;j <= 9;j++) res[j-i-1] = res[j];
  res[9-i] = 0;
  return res;
}

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent){
 HDC hdcMem, hdcMem2;
 HBITMAP hbmMask, hbmOld, hbmOld2;
 BITMAP bm;

 GetObject(hbmColour, sizeof(BITMAP), &bm);
 hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

 hdcMem = CreateCompatibleDC(NULL);
 hdcMem2 = CreateCompatibleDC(NULL);

 hbmOld = (HBITMAP)SelectObject(hdcMem, hbmColour);
 hbmOld2 = (HBITMAP)SelectObject(hdcMem2, hbmMask);

 SetBkColor(hdcMem, crTransparent);

 BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
 BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

 SelectObject(hdcMem, hbmOld);
 SelectObject(hdcMem2, hbmOld2);
 DeleteDC(hdcMem);
 DeleteDC(hdcMem2);

 return hbmMask;
}

void WriteInConsole(char* x){
     
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
 
    DWORD X;

    WriteConsole(hOut,x,strlen(x),&X,NULL);   
      
}

int strtoint(char* x){
  bool pos;
  int i = 0;
  if (x[0] == '-'){
    pos = false;
    i++;
  } else if (x[0] == '+'){
    pos = true;
    i++;
  }
  int res = 0;
  while (x[i] != 0){
    res *= 10;
    res += x[i] - '0';
    i++;
  }
  if (!pos) res = -res;
  return res;
}

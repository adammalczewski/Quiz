#include <windows.h>
#include <psapi.h>
#include "engine.h"

void ShowMessage(char* msg){
  MessageBox(NULL,msg,"Message",MB_OK);   
}

SStr inttostr(int x){
  SStr res;
  int i,j,arr[15];
  if (x == 0){
    res.s[0] = '0';
    res.s[1] = 0;
    return res;
  }
  if (x < 0){
    res.s[0] = '-';
    x = -x;
    j = 1;
  } else j = 0;
  i = 0;
  while (x > 0){
    arr[i++] = x%10;
    x/=10;      
  }
  while (i--){
    res.s[j++] = '0'+arr[i];    
  }
  res.s[j] = '\0';
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

TStream TStream::operator<<(RECT rc){
  (*this) << " left = " << inttostr(rc.left).s << "  right = " << inttostr(rc.right).s << 
  "  top = " << inttostr(rc.top).s << "  bottom = " << inttostr(rc.bottom).s;
}

TStream TStream::operator<<(int x){
  (*this) << inttostr(x).s;	
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

int Max(int x,int y){
  return (x > y)?x:y;	
}

int Min(int x,int y){
  return (x < y)?x:y;
}

double Maxd(double x,double y){
  return (x > y)?x:y;	
}

double Mind(double x,double y){
  return (x < y)?x:y;
}

RECT EmptyRect(){
  RECT rc;
  rc.left = 0;
  rc.right = 0;
  rc.top = 0;
  rc.bottom = 0;
  return rc;	
}

void CoutOn(){
  COUT = true;
  coutn++;  	
}

void CoutOff(){
  coutn = max(0,coutn-1);
  if (!coutn) COUT = false;	
}

int PrintMemoryInfo( DWORD processID ){
	
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    // Print the process identifier.

    cout <<  "\nProcess ID: " << inttostr(processID).s << "\n";

    // Print information about the memory usage of the process.

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID );
    if (NULL == hProcess) return 0;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) ){
        cout << "\tPageFaultCount: " <<  inttostr(pmc.PageFaultCount).s << "\n";
        cout <<  "\tPeakWorkingSetSize: " <<  inttostr(pmc.PeakWorkingSetSize).s << "\n";
        cout << "\tWorkingSetSize: " <<  inttostr(pmc.WorkingSetSize).s << "\n";
        cout << "\tQuotaPeakPagedPoolUsage: " << inttostr(pmc.QuotaPeakPagedPoolUsage).s << "\n";
        cout << "\tQuotaPagedPoolUsage: " << inttostr(pmc.QuotaPagedPoolUsage).s << "\n";
        cout << "\tQuotaPeakNonPagedPoolUsage: " << inttostr(pmc.QuotaPeakNonPagedPoolUsage).s << "\n";
        cout << "\tQuotaNonPagedPoolUsage: " << inttostr(pmc.QuotaNonPagedPoolUsage).s << "\n";
        cout << "\tPagefileUsage: " << inttostr(pmc.PagefileUsage).s << "\n";
        cout << "\tPeakPagefileUsage: " << inttostr(pmc.PeakPagefileUsage).s << "\n";
    }
    
    return pmc.PagefileUsage;

    CloseHandle( hProcess );
}

int MemoryUsage(HWND hwnd){

  DWORD processID;
  GetWindowThreadProcessId(hwnd, &processID);

  return PrintMemoryInfo(processID);

}

int FindFiles(const char *x){

UINT counter(0);
bool working(true);
char buffer[300];
char fileName[100][300];

WIN32_FIND_DATA myimage;
HANDLE myHandle=FindFirstFile(x,&myimage);

if(myHandle!=INVALID_HANDLE_VALUE){

       strcpy(&buffer[0],myimage.cFileName);
       strcpy(&fileName[counter][0],&buffer[0]);
       

       while(working)
       {
              FindNextFile(myHandle,&myimage);
              if(strcmp(&myimage.cFileName[0],&buffer[0])){

                     strcpy(&buffer[0],myimage.cFileName);
                     ++counter;
                     strcpy(&fileName[counter][0],&buffer[0]);

              }
              else{
                      //end of files reached
                      ++counter;
                      working=false;
              }

       }

  for (int i = 0;i < counter;++i) cout << fileName[i] << "\n";
}


}


char* OpenFile(HWND hwnd){
  OPENFILENAME ofn;       // common dialog box structure
char* szFile = new char[260];     // buffer for file name

// Initialize OPENFILENAME
ZeroMemory(&ofn, sizeof(ofn));
ofn.lStructSize = sizeof(ofn);
ofn.hwndOwner = hwnd;
ofn.lpstrFile = szFile;
// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
// use the contents of szFile to initialize itself.
ofn.lpstrFile[0] = '\0';
ofn.nMaxFile = 260;
ofn.lpstrFilter = "Tests\0*.tst\0All\0*.*\0";
ofn.nFilterIndex = 1;
ofn.lpstrFileTitle = NULL;
ofn.nMaxFileTitle = 0;
ofn.lpstrInitialDir = NULL;
ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn)==TRUE) return szFile;
  else {
	cout << CommDlgExtendedError() << "\n";
	cout << "Nie udalo sie otworzyc\n";
    delete szFile;
    return NULL;	
  }
}


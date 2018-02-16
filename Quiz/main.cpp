#include "engine.h"
#include <windows.h>
#include "Resources.h"
#include "TTestForm.h"
#include <Commctrl.h>

#define ID_OFBTN 1001

HWND hwnd;

bool COUT;
int coutn;
TStream cout;

TTestForm *TF;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND HButton;

PQuestion Q[MAX_QUESTIONS];
TTestInfo TestInfo;

int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nFunsterStil){
                   
    //AllocConsole();
    InitCommonControls();
    
    /* Creating Main Window */
                   
    WNDCLASSEX Window; 

    Window.hInstance = hInstance;
    Window.lpszClassName = "MyWindow";
    Window.lpfnWndProc = WndProc; 
    Window.style = 0;
    Window.cbSize = sizeof (WNDCLASSEX);
    Window.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    Window.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    Window.hCursor = LoadCursor (NULL, IDC_ARROW);
    Window.lpszMenuName = NULL; 
    Window.cbClsExtra = 0;    
    Window.cbWndExtra = 0;      
    Window.hbrBackground = CreateSolidBrush(RGB(236,233,216));

    if (!RegisterClassEx(&Window)) return 0;
    
    hwnd = CreateWindowEx(0,"MyWindow","Title",WS_BORDER|WS_SYSMENU|WS_MINIMIZEBOX,200,200,240,117,NULL,NULL,hInstance,NULL);
    HButton = CreateWindow("BUTTON","Open File",WS_CHILD|WS_VISIBLE,20,20,200,50,hwnd,(HMENU)ID_OFBTN,hInstance,NULL);
    
    ShowWindow (hwnd, nFunsterStil);
    
    /* Registering TestForm */
    
    if (!RegisterTestForm(hInstance)) return 0;
    
    if (!RegisterClientButton(hInstance)){
	  cout << "Nie udalo sie zarejestrowac klasy guzika\n";
	  return 0;
	};
    
    /* Showing TestForm */
    
    TF = new TTestForm(hInstance,hwnd,0,0);
    
    MSG msg;

    while (GetMessage (&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //FreeConsole();
    return msg.wParam;
    
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message){
		/* Messages from TestForm */
        case (WM_USER+MWM_DESTROY):{
		  cout << "Test Windows was abruptly closed\n";
          PostQuitMessage(0);  
        } break;
        case (WM_USER+MWM_TESTEND):
		  cout << "Test Ended\n";
		  TF->Hide();
		  ShowWindow(hwnd,SW_SHOW);
		  PostQuitMessage(0);
		  break;
		/* ... */
		case WM_COMMAND:{
		  CoutOn();
		  if (LOWORD(wParam) == ID_OFBTN){
            char *FileName = OpenFile(hwnd);
            if (FileName != NULL){
			  cout << "Nie NULL\n";
			  TF->Show();
			  ShowWindow(hwnd,SW_HIDE);
			  
			  /* Loading Questions' File */

              if (!LoadFromFile(FileName,&TestInfo,&Q[0])){
	            cout << "Blad przy wczytywaniu\n";
	            return 0;
	          }
	          
	          TF->PrepareTest(TestInfo,Q);
	          TF->StartTest();
	          
			}
		  }
		  CoutOff();
       } break; case WM_DESTROY:{
          PostQuitMessage (0);
          break;
        } break; default:
          return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

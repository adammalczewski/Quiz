#include <windows.h>
#include "ClientButton.h"
#include "ClientButtonRes.h"
#include "engine.h"

TClientButton::TClientButton(HINSTANCE HInstance,HWND Parent,int x,int y,int ID,bool show,bool big){
  cout << "In TClientButton::TClientButton() :\n";
  if (!Big) hwnd = CreateWindow("ClientButton","",WS_CHILD|WS_VISIBLE,x,y,BUTTONW,BUTTONH,Parent,(HMENU)ID,HInstance,NULL);
  else hwnd = CreateWindow("ClientButton","",WS_CHILD|WS_VISIBLE,x,y,BIGBUTTONW,BIGBUTTONH,Parent,(HMENU)ID,HInstance,NULL);
  if (!big) HRgn = CreateRectRgn(0,0,BUTTONW,BUTTONH);
  HRgn = CreateRectRgn(0,0,BIGBUTTONW,BIGBUTTONH);
  SetWindowRgn(hwnd,HRgn,false);  
  this->ID = ID;
  if (!show) ShowWindow(hwnd,SW_HIDE);
  HDC hdc = GetDC(hwnd);
  Buffer = CreateCompatibleDC(hdc);
  Buffer2 = CreateCompatibleDC(hdc);
  BHbm = (HBITMAP)LoadImage(HInstance,MAKEINTRESOURCE(BUTTON),IMAGE_BITMAP,0,0,LR_SHARED);
  SelBHbm = (HBITMAP)LoadImage(HInstance,MAKEINTRESOURCE(SELBUTTON),IMAGE_BITMAP,0,0,LR_SHARED);
  BigBHbm = (HBITMAP)LoadImage(HInstance,MAKEINTRESOURCE(BIGBUTTON),IMAGE_BITMAP,0,0,LR_SHARED);
  BigSelBHbm = (HBITMAP)LoadImage(HInstance,MAKEINTRESOURCE(BIGSELBUTTON),IMAGE_BITMAP,0,0,LR_SHARED);
  if (!big) BufferHbm = CreateCompatibleBitmap(hdc,BUTTONW,BUTTONH);
  else BufferHbm = CreateCompatibleBitmap(hdc,BIGBUTTONW,BIGBUTTONH);
  Big = big;
  BufferOldHbm = (HBITMAP)SelectObject(Buffer,BufferHbm);
  Buffer2OldHbm = (HBITMAP)SelectObject(Buffer2,BHbm);
  TextFont = CreateFont(20,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");
  ReleaseDC(hwnd,hdc);
  SetWindowLong(hwnd,0,(LONG)this);
  cout << "~TClientButton::TClientButton()\n";
}

TClientButton::~TClientButton(){
  cout << "In TClientButton::~TClientButton() :\n";
  SelectObject(Buffer,BufferOldHbm);
  SelectObject(Buffer2,Buffer2OldHbm);
  DeleteObject(BufferHbm);
  DeleteObject(BHbm);
  DeleteObject(SelBHbm);
  DeleteObject(BigBHbm);
  DeleteObject(BigSelBHbm);
  DeleteObject(Buffer);
  DestroyWindow(hwnd); 
  cout << "~TClientButton::~TClientButton()\n";      
}

void TClientButton::SetText(SStr Text){
  this->Text = Text;
}

void TClientButton::Show(){
  ShowWindow(hwnd,SW_SHOW);            
}

void TClientButton::Hide(){
  ShowWindow(hwnd,SW_HIDE);
}

void TClientButton::Redraw(){
  cout << "In TClientButton::Redraw() :\n";
  HDC hdc = GetDC(hwnd);
  Draw(hdc);
  ReleaseDC(hwnd,hdc); 
  cout << "~TClientButton::Redraw()\n";  
}

void TClientButton::Draw(HDC hdc){
  cout << "In TClientButton::Draw() :\n";
  
  RECT rc;
  rc.left = 0;
  rc.top = 0;
  if (!Big){
    rc.bottom = BUTTONH;
    rc.right = BUTTONW;
  } else {
    rc.bottom = BIGBUTTONH;
	rc.right = BIGBUTTONW;		
  }
  
  SetBkMode(Buffer,TRANSPARENT);
  
  HFONT OldFont = (HFONT)SelectObject(Buffer,TextFont);
  
  if (!Big){
    if (!sel){
      SelectObject(Buffer2,BHbm);
      BitBlt(Buffer,0,0,BUTTONW,BUTTONH,Buffer2,0,0,SRCCOPY);
      DrawText(Buffer,Text.s,-1,&rc,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
    } else {
      SelectObject(Buffer2,SelBHbm);
      BitBlt(Buffer,0,0,BUTTONW,BUTTONH,Buffer2,0,0,SRCCOPY);   
	  DrawText(Buffer,Text.s,-1,&rc,DT_CENTER|DT_SINGLELINE|DT_VCENTER); 
    }
  } else {
    if (!sel){
      SelectObject(Buffer2,BigBHbm);
      BitBlt(Buffer,0,0,BIGBUTTONW,BIGBUTTONH,Buffer2,0,0,SRCCOPY);
      DrawText(Buffer,Text.s,-1,&rc,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
    } else {
      SelectObject(Buffer2,BigSelBHbm);
      BitBlt(Buffer,0,0,BIGBUTTONW,BIGBUTTONH,Buffer2,0,0,SRCCOPY);   
	  DrawText(Buffer,Text.s,-1,&rc,DT_CENTER|DT_SINGLELINE|DT_VCENTER); 
    } 		
  }
  
  if (!Big) BitBlt(hdc,0,0,BUTTONW,BUTTONH,Buffer,0,0,SRCCOPY);
  else BitBlt(hdc,0,0,BIGBUTTONW,BIGBUTTONH,Buffer,0,0,SRCCOPY);
  
  SelectObject(Buffer,OldFont);
  
  cout << "In TClientButton::~Draw()\n";
}

void TClientButton::Move(int x,int y){
  if (!Big) MoveWindow(hwnd,x,y,BUTTONW,BUTTONH,false);
  else MoveWindow(hwnd,x,y,BIGBUTTONW,BIGBUTTONH,false);
  RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE);	
}

bool RegisterClientButton(HINSTANCE HInstance){
  
  WNDCLASS Window;   
  Window.hInstance = HInstance;
  Window.lpszClassName = "ClientButton";
  Window.lpfnWndProc = CBWndProc; 
  Window.style = 0;
  Window.hIcon = NULL;
  Window.hCursor = NULL;
  Window.lpszMenuName = NULL; 
  Window.cbClsExtra = 0;
  Window.cbWndExtra = 8;
  Window.hbrBackground = (HBRUSH) GetStockObject (NULL_BRUSH);
  
  if (!RegisterClass(&Window)) return false;
  else return true; 
              
}

LRESULT CALLBACK CBWndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam){
  cout << "In CBWndProc() :\n";
  switch (Msg){
    case WM_CREATE:{

    } break;
    case WM_LBUTTONDOWN:{
	  cout << "WM_LBUTTONDOWN\n";
      TClientButton* CB = (TClientButton*)GetWindowLong(hwnd,0);
      cout << "Parent = " << inttostr(int(GetParent(hwnd))).s << "\n";
      if (CB != NULL) SendMessage(GetParent(hwnd),WM_COMMAND,MAKELONG(CB->ID,0),(LPARAM)hwnd);
      cout << "~WM_LBUTTONDOWN\n";
    } break;
    case WM_PAINT:{
	  cout << "WM_PAINT\n";
      TClientButton* CB = (TClientButton*)GetWindowLong(hwnd,0);   
      if (CB != NULL){
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd,&ps);
        CB->Draw(hdc);
        EndPaint(hwnd,&ps);
      }
      cout << "~WM_PAINT\n";
    } break;
    case WM_MOUSEMOVE:{
      TClientButton* CB = (TClientButton*)GetWindowLong(hwnd,0);
      if (CB != NULL){
        if (!CB->sel){
          SetCapture(hwnd);
          CB->sel = true;
          InvalidateRgn(hwnd,NULL,true);        
        }
        POINT p;
        GetCursorPos(&p);
        ScreenToClient (hwnd, &p); 
        HRGN HRgn;
        if (!CB->Big) HRgn = CreateRectRgn(0,0,BUTTONW,BUTTONH);
        else HRgn = CreateRectRgn(0,0,BIGBUTTONW,BIGBUTTONH);
        if (!PtInRegion(HRgn,p.x,p.y)){
          CB->sel = false;
          InvalidateRgn(hwnd,NULL,true);
          ReleaseCapture();                               
        }  
		DeleteObject(HRgn);  
      }
    } break;
    case WM_CAPTURECHANGED:{
      TClientButton* CB = (TClientButton*)GetWindowLong(hwnd,0);
      if (CB != NULL){
        CB->sel = false;
        InvalidateRgn(hwnd,NULL,true);  
      }   
    } break;
    default: 
	  cout << "~CBWndProc()\n";
	  return DefWindowProc(hwnd,Msg,wParam,lParam);
  } 
  cout << "~CBWndProc()\n";
  return 0;           
}

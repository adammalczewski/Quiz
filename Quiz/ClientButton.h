#include "engine.h"
#include <windows.h>

#define BUTTONW 120
#define BUTTONH 38
#define BIGBUTTONW 222
#define BIGBUTTONH 70

class TClientButton{
  public: 
    TClientButton(HINSTANCE HInstance,HWND Parent,int x,int y,int ID,bool show,bool big);
    ~TClientButton();
    int GetID(){return ID;}
    void Show();
    void Hide();
    void Redraw();
    void Move(int x,int y);
    void SetText(SStr Text);
  private:
    friend LRESULT CALLBACK CBWndProc(HWND, UINT, WPARAM,LPARAM);
    HRGN HRgn;
    bool Big;
    HFONT TextFont;
    SStr Text;
    HWND hwnd;
    int ID;
    HDC Buffer,Buffer2;
    bool sel;
    HBITMAP BHbm,SelBHbm,BigBHbm,BigSelBHbm,BufferHbm,BufferOldHbm,Buffer2OldHbm;
    void Draw(HDC hdc);
};

bool RegisterClientButton(HINSTANCE HInstance);
LRESULT CALLBACK CBWndProc(HWND, UINT, WPARAM, LPARAM);

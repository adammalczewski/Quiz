#include "engine.h"
#include "Resources.h"
#include <windows.h>
#include <ctime>
#include "TTestForm.h"

#define S_NOTHING 0      /* TestForm state */
#define S_ANIMATION1 1
#define S_QUESTION 2
#define S_ANSWERCHECK 3
#define S_ANIMATION2 4
#define S_SANIMATION 5
#define S_SUMMARY 6
#define S_CTANIMATION1 7
#define S_CTWAITING 8
#define S_CTANIMATION2 9

#define SS_ANSWERS1 0    /* Summary State */
#define SS_ANSWERS2 1
#define SS_SPEED1 2
#define SS_SPEED2 3
#define SS_ALL1 4
#define SS_ALL2 5

#define AC_SHOWING 0     /* Answer Check State */
#define AC_WAITING 1
#define AC_HIDING 2

#define AC_LEFT 35       /* Answer Check Box Position */
#define AC_TOP 230 

#define CT_LEFT 35       /* Correction Tests Box Position */
#define CT_TOP 230

#define AM_BAD 0         /* Answer Mark (wrong,right,acceptable but with minor mistakes) */
#define AM_GOOD 1
#define AM_MEDIOCRE 2

#define TIMERX 7         /* Timer Position */
#define TIMERY 7

#define TIMERSIZE 130    /* Timer Size */

#define QLEFT 150        /* Question Position */
#define QRIGHT 630
#define QTOP 7
#define QBOTTOM 137

#define EDIT_LEFT 70     /* Edit Position (Where user is writing answers to TByHandQ and TEstimationQ) */
#define EDIT_TOP 350
#define EDIT_WIDTH 500
#define EDIT_HEIGHT 27

#define TIP_TOP 200      /* Tip position (In TByHandQ)*/

#define ANIMATIONTIME 0.6      //Time for question to appear or dissappear
#define SANIMATIONTIME 6       //Time for whole summary animation
#define ACANIMATIONTIME 0.5   //one-way animation of appearing or dissappearing Check Answer Box
#define ACWAITINGTIME 2      //Time for Check Answer Box to wait

#define CTANIMATIONTIME 0.5 //one-way animation of appearing or dissappearing Correction Tests Box

#define ANSWERSPTS_LEFT 30     //Summary positions
#define ANSWERSPTS_TOP 100
#define SPEEDPTS_LEFT 30
#define SPEEDPTS_TOP 200
#define ALLPTS_LEFT 30
#define ALLPTS_TOP 300

#define POINTS_WIDTH 300       //Summary gauges' sizes
#define POINTS_HEIGHT 30

#define AC_WIDTH 570 //Check Answer Box Width
#define AC_HEIGHT 150

#define CT_WIDTH 570 //Correction Tests Box Width
#define CT_HEIGHT 150

#define CT_POSITIVE 0
#define CT_NEGATIVE 1
#define CT_ALL 2
#define CT_CHOOSE 3

#define AC_TICKWIDTH 100
#define AC_TICKHEIGHT 138

#define AC_CROSSWIDTH 100
#define AC_CROSSHEIGHT 132

#define ID_EXITBTN 3000
#define ID_CTESTSBTN 3001
#define ID_POSITIVEBTN 3002
#define ID_NEGATIVEBTN 3003
#define ID_ALLBTN 3004
#define ID_CHOOSEBTN 3005

bool Registered;

TTestForm::TTestForm(HINSTANCE HInst,HWND Parent,int x,int y){
  cout << "TTestForm()\n";
  hwnd = CreateWindowEx(0,"TestForm","Test",WS_BORDER|WS_SYSMENU|WS_MINIMIZEBOX|WS_EX_COMPOSITED,200,100,TESTFORMW+GetSystemMetrics(SM_CXFIXEDFRAME)*2,TESTFORMH+GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYCAPTION),NULL,NULL,HInst,NULL);
  HEdit = CreateWindow("EDIT","",WS_CHILD|WS_BORDER,0,0,EDIT_WIDTH,EDIT_HEIGHT,hwnd,NULL,HInst,NULL);
  WNDPROC DefEditProc;
  DefEditProc = (WNDPROC)SetWindowLong (HEdit, GWL_WNDPROC, (LONG)MyEditProc);
  SetWindowLong(HEdit,GWL_USERDATA,(LONG)DefEditProc);
  HEditFont = CreateFont(24,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");
  SendMessage(HEdit,WM_SETFONT,(LPARAM)HEditFont,0);
  SetWindowLong(hwnd,0,(LONG)this);
  HDC hdc = GetDC(hwnd);
  this->Parent = Parent;
  Buffer = CreateCompatibleDC(hdc);
  Buffer2 = CreateCompatibleDC(hdc);
  BufferHbm = CreateCompatibleBitmap(hdc,TESTFORMW,TESTFORMH);
  HbmOldBuffer = (HBITMAP)SelectObject(Buffer,BufferHbm);
  HbmOldBuffer2 = (HBITMAP)SelectObject(Buffer2,NULL);
  ReleaseDC(hwnd,hdc);
  State = S_NOTHING;
  Questions = NULL;
  ButtonsFont = CreateFont(19,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Times New Roman");
  TimerFont = CreateFont(70,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");
  QFont = CreateFont(40,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");
  ACPointsFont = CreateFont(40,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");
  EnterFont = CreateFont(24,0,0,0,FW_THIN,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");
  InstantAnswering = true;
  ShowAnswer = true;
  ShuffleQ = true;
  Enter = true;
  CorrectionTests = true;
  ExitBtn = new TClientButton(HInst,hwnd,TESTFORMW-BUTTONW-10,TESTFORMH-BUTTONH-6,ID_EXITBTN,false,false);
  CTestsBtn = new TClientButton(HInst,hwnd,TESTFORMW-BUTTONW-10,TESTFORMH-2*BUTTONH-16,ID_CTESTSBTN,false,false);
  PositiveBtn = new TClientButton(HInst,hwnd,TESTFORMW,TESTFORMH,ID_POSITIVEBTN,false,true);
  NegativeBtn = new TClientButton(HInst,hwnd,TESTFORMW,TESTFORMH,ID_NEGATIVEBTN,false,true);
  AllBtn = new TClientButton(HInst,hwnd,TESTFORMW,TESTFORMH,ID_ALLBTN,false,true);
  ChooseBtn = new TClientButton(HInst,hwnd,TESTFORMW,TESTFORMH,ID_CHOOSEBTN,false,true);
  SStr S;
  strcpy(S.s,"Exit");
  ExitBtn->SetText(S);
  strcpy(S.s,"Correction Tests");
  CTestsBtn->SetText(S);
  strcpy(S.s,"Positive");
  PositiveBtn->SetText(S);
  strcpy(S.s,"Negative");
  NegativeBtn->SetText(S);
  strcpy(S.s,"All");
  AllBtn->SetText(S);
  strcpy(S.s,"Custom");
  ChooseBtn->SetText(S);
  EnterRgn = NULL;
  InEnter = false;
  cout << "~TTestForm()\n\n";
}

void TTestForm::Show(){
  ShowWindow(hwnd,SW_SHOW);                  
}

void TTestForm::Hide(){
  ShowWindow(hwnd,SW_HIDE);                  
}

bool TTestForm::PrepareTest(TTestInfo TestInfo,PQuestion* Qs){
  cout << "In PrepareTest() :\n";
  this->TestInfo = TestInfo;
  this->TestInfo.ABCDtime = 60;
  Questions = Qs;       
  OwnQs = false;
  cout << "~PrepareTest()\n\n";
}

bool TTestForm::StartTest(){
  if ((Questions != NULL) && (TestInfo.QuestionsC != 0)){
    if (ShuffleQ && (TestInfo.QuestionsC > 2)){
      for (int i = 0;i < TestInfo.QuestionsC*10;i++){
        int x = rand()%TestInfo.QuestionsC;
        int y = x;
        while (x == y) y = rand()%TestInfo.QuestionsC;
        PQuestion Q = Questions[x];
        Questions[x] = Questions[y];
        Questions[y] = Q;    
      }             
    }
    SetQuestion(0);
    if (!SetTimer(hwnd,1,1,NULL)) return false;
    if (QueryPerformanceFrequency (&Freq) == 0){
      KillTimer(hwnd,1);
      return false;                              
    }
    ts = GetSecs();
    State = S_ANIMATION1; 
    SAnswersPtsAll = 0;
    SAnswersPtsMax = 0;
    SAnswersPtsMin = 0;
    SSpeedPtsAll = 0;
    SSpeedPtsMax = 0;
  } else return false;
}

double TTestForm::GetSecs(){
  LARGE_INTEGER uTicks;
  QueryPerformanceCounter (&uTicks);
  return (double)(uTicks.QuadPart / (double)Freq.QuadPart);
}

char* TTestForm::GenerateTip(char* Answer){
  char* result = new char[100];
  strcpy(result,"");
  int l = strlen(Answer);
  int spaces = 0;
  for (int i = 0;i < l;i++){
    if (Answer[i] == ' '){
      strcat(result," ");
      spaces++;
    }else if (Tip[i-spaces]){
      char temp = Answer[i+1];
      Answer[i+1] = 0;
      strcat(result,&Answer[i]);
      Answer[i+1] = temp;
    } else {
      strcat(result,"_");       
    }
    if (i < l-1) strcat(result," ");
  }
  return result;    
}

void TTestForm::SetQuestion(int q){
  cout << "SetQuestion(" << inttostr(q).s << ")\n" ;
  actual = q;
  type = Questions[q]->type;
  switch (type){
    case QT_ABCD:{
      if (Questions[q]->deftimeset){
        time = TestInfo.ABCDtime;                            
        freetime = TestInfo.ABCDft;
      } else {
        time = Questions[q]->time;
        freetime = Questions[q]->freetime;
      }
      strcpy(Q.s,Questions[q]->Q.s);
      IsTip = false;
      IsEdit = false;
      BtnsC = ((TABCDQ*)(Questions[q]))->AnswersC;
      for (int i = 0;i < BtnsC;i++){
        Btns[i].text = ((TABCDQ*)(Questions[q]))->As[i].s;
        Btns[i].checked = false;    
      }
      Clicked = -1;
      MouseOn = -1;
    } break;
    case QT_BYHAND:{
      if (Questions[q]->deftimeset){
        time = TestInfo.BHtime;                            
        freetime = TestInfo.BHft;
      } else {
        time = Questions[q]->time;
        freetime = Questions[q]->freetime;
      }
      strcpy(Q.s,Questions[q]->Q.s);
      
      TipStr = NULL;
      int i = 0;
      int space = 0;
      while (((TByHandQ*)(Questions[q]))->A.s[i]){
        i++;
        if (((TByHandQ*)(Questions[q]))->A.s[i] == ' ') space++;      
      }
      AnsLength = i-space;
      TipLength = -1;
      for (int i = 0;i < AnsLength;i++) Tip[i] = false;
      
      if (((TByHandQ*)(Questions[q]))->deftipset){
        IsTip = TestInfo.BHtip;
        TipStart = TestInfo.BHtipstart;
        TipMax = int((AnsLength+1)/TestInfo.BHtipcost)-1;
      } else {
        IsTip = ((TByHandQ*)(Questions[q]))->IsTip;
        TipStart = ((TByHandQ*)(Questions[q]))->TipStart;
        TipMax = int((AnsLength+1)/((TByHandQ*)(Questions[q]))->TipCost)-1;                              
      }
      IsEdit = true;
      BtnsC = 0;
    } break;
    case QT_GOODORBAD:{
      if (Questions[q]->deftimeset){
        time = TestInfo.GBtime;                            
        freetime = TestInfo.GBft;
      } else {
        time = Questions[q]->time;
        freetime = Questions[q]->freetime;
      }
      strcpy(Q.s,Questions[q]->Q.s);
      IsTip = false;
      IsEdit = false;
      BtnsC = ((TGoodorBadQ*)(Questions[q]))->AnswersC;
      for (int i = 0;i < BtnsC;i++){
        Btns[i].text = ((TGoodorBadQ*)(Questions[q]))->As[i].s;
        Btns[i].checked = false;   
      }
      Clicked = -1;
      MouseOn = -1;   
    } break;
    case QT_ESTIMATION:{
      cout << "QT_ESTIMATION\n";
      if (Questions[q]->deftimeset){
        time = TestInfo.Etime;                            
        freetime = TestInfo.Eft;
      } else {
        time = Questions[q]->time;
        freetime = Questions[q]->freetime;
      }
      strcpy(Q.s,Questions[q]->Q.s);
      IsTip = false;
      IsEdit = true;
      BtnsC = 0;   
    } break;       
  }  
  SetPositions();
  cout << "~SetQuestion\n";
}

void TTestForm::SetPositions(){ //BtnsC and Btns[].text must be proper 
  
  cout << "SetPosition()\n";
  
  HDC hdc = GetDC(hwnd);
  
  // Question
  HFONT HOldFont = (HFONT)SelectObject(hdc,QFont);
  RECT Rect;
  Rect.top = 0;
  Rect.bottom = 0;
  Rect.left = QLEFT;
  Rect.right= QRIGHT;
  DrawText(hdc,Q.s,-1,&Rect,DT_WORDBREAK|DT_CENTER|DT_CALCRECT);
  Rect.top = QTOP + int((QBOTTOM-QTOP-Rect.bottom)/2);
  Rect.bottom += Rect.top;
  Rect.left = QLEFT + int((QRIGHT-QLEFT-Rect.right+Rect.left)/2);
  Rect.right += Rect.left - QLEFT;
  
  QSRect = Rect;
  OffsetRect(&Rect,TESTFORMW-Rect.left,0);
  QFRect = Rect;
  QRect = QFRect;
  
  //Tip
  if (IsTip){
    TipStr = GenerateTip(((TByHandQ*)(Questions[actual]))->A.s);
    Rect.top = TIP_TOP;
    Rect.bottom = TIP_TOP;
    Rect.left = 0;
    Rect.right = TESTFORMW;
    TipRect = Rect;
    int i = 30;
    TipFont = CreateFont(i,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");
    SelectObject(hdc,TipFont);
    DrawText(hdc,TipStr,-1,&Rect,DT_CALCRECT);
    while (Rect.right - Rect.left > TESTFORMW){
      i--;
      DeleteObject(TipFont);
      TipFont = CreateFont(i,0,0,0,FW_BOLD,false,false,false,EASTEUROPE_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"");  
      SelectObject(hdc,TipFont);
      DrawText(hdc,TipStr,-1,&Rect,DT_CALCRECT);
    }
    TipRect.bottom = Rect.bottom;
  }
  
  //Buttons
  if (BtnsC){
  int maxy[10];
  int maxx[2];
  for (int i = 0;i < 2;i++) maxx[i] = 0;
  for (int i = 0;i < 10;i++) maxy[i] = 0;
  SelectObject(hdc,ButtonsFont);
  int posx1,posx2,posy,fully(0),b;
  for (int i = 0;i < BtnsC;i++){
    RECT rect;
    rect.left = 0;
    rect.right = 280;
    rect.top = 0;
    rect.bottom = 19;
    if (BtnsC <= 6){
      DrawText(hdc,Btns[i].text,-1,&rect,DT_CALCRECT);
      maxy[i] = Max(16,rect.bottom-rect.top);
      maxx[0] = Max(maxx[0],rect.right-rect.left);
    } else {
      DrawText(hdc,Btns[i].text,-1,&rect,DT_WORDBREAK|DT_CALCRECT);
      maxy[int(i/2)] = Max(16,Max(maxy[int(i/2)],rect.bottom-rect.top));
      maxx[i%2] = Max(maxx[i%2],rect.right-rect.left);
    }                    
    Btns[i].FTextRect = rect;
  }
  for (int i = 0;i < 10;i++) fully += maxy[i];
  b = Min((448-fully)/((BtnsC+1)/2-1),20);
  posy = (448-fully-b*((BtnsC+1)/2-1))/2+154;
  if (BtnsC <= 6){
    posx1 = ((620-maxx[0]-20)/2)+10;
    int y = posy;
    int fx,sx;
    fx = posx1-320-int((maxx[0]+20)/2);
    sx = posx1+320+int((maxx[0]+20)/2);
    for (int i = 0;i < BtnsC;i++){
      if (i%2) Btns[i].fx = sx;
      else Btns[i].fx = fx;
      Btns[i].sx = posx1;
      Btns[i].fy = y+int((maxy[i]-16)/2);
      Btns[i].sy = Btns[i].fy;
      Btns[i].x = Btns[i].fx;
      Btns[i].y = Btns[i].fy;
      OffsetRect(&Btns[i].FTextRect,Btns[i].fx+20,y);
      Btns[i].STextRect = Btns[i].FTextRect;
      OffsetRect(&Btns[i].STextRect,Btns[i].sx-Btns[i].fx,Btns[i].sy-Btns[i].fy);
      Btns[i].TextRect = Btns[i].FTextRect;
      y += maxy[i]+b;
    }      
  } else {
    posx1 = ((300-maxx[0]-20)/2)+10;
    posx2 = ((300-maxx[1]-20)/2)+330;
    int y = posy;
    int fx,sx;
    fx = posx1-160-int((Max(maxx[0],maxx[1])+20)/2);
    sx = posx2+160+int((Max(maxx[0],maxx[1])+20)/2);
    for (int i = 0;i < BtnsC;i++){
      if (i%2){
        Btns[i].fx = sx;
        Btns[i].sx = posx2;
        Btns[i].fy = y+int((maxy[i/2]-16)/2);
        Btns[i].sy = Btns[i].fy;
        Btns[i].x = Btns[i].fx;
        Btns[i].y = Btns[i].fy;
        OffsetRect(&Btns[i].FTextRect,Btns[i].fx+20,y);
        Btns[i].STextRect = Btns[i].FTextRect;
        OffsetRect(&Btns[i].STextRect,Btns[i].sx-Btns[i].fx,Btns[i].sy-Btns[i].fy);
        Btns[i].TextRect = Btns[i].FTextRect;
        y += maxy[i/2]+b;  
      } else {
        Btns[i].fx = fx;
        Btns[i].sx = posx1;
        Btns[i].fy = y+int((maxy[i/2]-16)/2);
        Btns[i].sy = Btns[i].fy;
        Btns[i].x = Btns[i].fx;
        Btns[i].y = Btns[i].fy;
        OffsetRect(&Btns[i].FTextRect,Btns[i].fx+20,y);
        Btns[i].STextRect = Btns[i].FTextRect;
        OffsetRect(&Btns[i].STextRect,Btns[i].sx-Btns[i].fx,Btns[i].sy-Btns[i].fy);   
        Btns[i].TextRect = Btns[i].FTextRect;   
      }  
    }
  }
  }
  
  //Edit
  if (IsEdit){
    editfx = EDIT_LEFT;
    editsx = EDIT_LEFT;
    editfy = TESTFORMH;
    editsy = EDIT_TOP;
    SetWindowText(HEdit,"");
    ShowWindow(HEdit,SW_SHOW);
  }
  
  
  
  SelectObject(hdc,HOldFont);
  ReleaseDC(hwnd,hdc);
  cout << "~SetPositions()\n";
  
}

bool TTestForm::UpdatePositions1(){
     
  double pos = Mind((lt-ts)/(ANIMATIONTIME),1.0);
  int w,h;
  
  //Question
  w = QRect.right - QRect.left;
  h = QRect.bottom - QRect.top;
  QRect.top = int(QFRect.top*(1-pos) + QSRect.top*pos);
  QRect.left = int(QFRect.left*(1-pos) + QSRect.left*pos);
  QRect.bottom = QRect.top + h;
  QRect.right = QRect.left + w;
  
  //Buttons
  for (int i = 0;i < BtnsC;i++){
    Btns[i].x = int(Btns[i].fx*(1-pos) + Btns[i].sx*pos);
    Btns[i].y = int(Btns[i].fy*(1-pos) + Btns[i].sy*pos);
    w = Btns[i].TextRect.right - Btns[i].TextRect.left;
    h = Btns[i].TextRect.bottom - Btns[i].TextRect.top;
    Btns[i].TextRect.top = int(Btns[i].FTextRect.top*(1-pos) + Btns[i].STextRect.top*pos);
    Btns[i].TextRect.left = int(Btns[i].FTextRect.left*(1-pos) + Btns[i].STextRect.left*pos);
    Btns[i].TextRect.bottom = Btns[i].TextRect.top + h;
    Btns[i].TextRect.right = Btns[i].TextRect.left + w;
  }
  
  //Edit
  if (IsEdit) MoveWindow(HEdit,int(editfx*(1-pos)+editsx*pos),int(editfy*(1-pos)+editsy*pos),EDIT_WIDTH,EDIT_HEIGHT,true);
  
  if (pos == 1) return true;
  else return false;
  
}

void TTestForm::SetRegions(){
  for (int i = 0;i < BtnsC;i++){
    HRGN RectRgn = CreateRectRgnIndirect(&Btns[i].TextRect);
    HRGN CircleRgn = CreateEllipticRgn(Btns[i].x,Btns[i].y,Btns[i].x+16,Btns[i].y+16);
    Btns[i].HRegion = CreateRectRgn(0,0,1,1);
    CombineRgn(Btns[i].HRegion,RectRgn,CircleRgn,RGN_OR);
  }
}

bool TTestForm::UpdatePositions2(){
     
  double pos = Mind((lt-ts)/(ANIMATIONTIME),1);
  int w,h;
  
  //Question
  w = QRect.right - QRect.left;
  h = QRect.bottom - QRect.top;
  QRect.top = int(QSRect.top*(1-pos) + QFRect.top*pos);
  QRect.left = int(QSRect.left*(1-pos) + QFRect.left*pos);
  QRect.bottom = QRect.top + h;
  QRect.right = QRect.left + w;
  
  //Buttons
  for (int i = 0;i < BtnsC;i++){
    Btns[i].x = int(Btns[i].sx*(1-pos) + Btns[i].fx*pos);
    Btns[i].y = int(Btns[i].sy*(1-pos) + Btns[i].fy*pos);
    w = Btns[i].TextRect.right - Btns[i].TextRect.left;
    h = Btns[i].TextRect.bottom - Btns[i].TextRect.top;
    Btns[i].TextRect.top = int(Btns[i].STextRect.top*(1-pos) + Btns[i].FTextRect.top*pos);
    Btns[i].TextRect.left = int(Btns[i].STextRect.left*(1-pos) + Btns[i].FTextRect.left*pos);
    Btns[i].TextRect.bottom = Btns[i].TextRect.top + h;
    Btns[i].TextRect.right = Btns[i].TextRect.left + w;
  }
  
  //Edit
  if (IsEdit) MoveWindow(HEdit,int(editsx*(1-pos)+editfx*pos),int(editsy*(1-pos)+editfy*pos),EDIT_WIDTH,EDIT_HEIGHT,true);
  
  if (pos == 1) return true;
  else return false;
       
}

void TTestForm::CheckAnswer(){
  cout << "CheckAnswer() :\n";
  int goodp,badp;
  int t = int(lt-ts);
  int timeb,freetime;
  Results[actual].Positive = false;
  
  switch (type){
    case QT_ABCD:{
	  cout << "QT_ABCD :\n";
      int i(0);
      for (i = 0;i < BtnsC;i++) if (Btns[i].checked) break;
      cout << "it's " << inttostr(i+1).s << " button that get clicked\n";
      cout << "Checking point settings ...\n";
      cout << "actual = " << inttostr(actual).s << "\n";
        if (Questions[actual]->defpointset){
          goodp = TestInfo.ABCDgp;
          badp = TestInfo.ABCDbp;                                 
        } else {
          goodp = Questions[actual]->goodp;
          badp = Questions[actual]->badp;
        } 
        cout << "Checking time settings ... \n";
        if (Questions[actual]->deftimepset){
          timeb = TestInfo.ABCDtimeb;
          freetime = TestInfo.ABCDft;                                  
        } else {
          timeb = Questions[actual]->timebonus;
          freetime = Questions[actual]->freetime;       
        }
      if (i != BtnsC){
        cout << "Now i'll give pointsjj\n";
        if (((TABCDQ*)Questions[actual])->a == i){
          SAnswersPtsAll += goodp;
          Results[actual].Positive = true;
        } else SAnswersPtsAll += badp;   
		  cout << inttostr(SAnswersPtsAll).s << " pts\n";
      } else { //Didnt choose answer
	   	SAnswersPtsAll+= badp;	
	  }
      SAnswersPtsMax += goodp;
      SAnswersPtsMin += badp;
      if (Results[actual].Positive) cout << "It's the good answer\n";
      else cout << "It's not the good answer\n";
      cout << "~QT_ABCD\n";
    } break;
    case QT_BYHAND:{
      if (Questions[actual]->defpointset){
        goodp = TestInfo.BHgp;
        badp = TestInfo.BHbp;                                   
      } else {
        goodp = Questions[actual]->goodp;
        badp = Questions[actual]->badp;
      }
      if (Questions[actual]->deftimepset){
        timeb = TestInfo.BHtimeb;
        freetime = TestInfo.BHft;
      } else {
        timeb = Questions[actual]->timebonus;
        freetime = Questions[actual]->freetime;       
      }
      DWORD x = GetWindowTextLength(HEdit);
      LPSTR temp = (LPSTR)GlobalAlloc(GPTR,x+1);
      GetWindowText(HEdit,temp,x+1);
      if (!strcmp(((TByHandQ*)Questions[actual])->A.s,temp)){
        SAnswersPtsAll += goodp;
        Results[actual].Positive = true;
      } else SAnswersPtsAll += badp;
      cout << ((TByHandQ*)Questions[actual])->A.s << "\n" << temp << "\n";
      SAnswersPtsMax += goodp;
      SAnswersPtsMin += badp;
      GlobalFree(temp);
    } break;
    case QT_GOODORBAD:{
      if (Questions[actual]->defpointset){
        goodp = TestInfo.GBgp;
        badp = TestInfo.GBbp;                                  
      } else {
        goodp = Questions[actual]->goodp;
        badp = Questions[actual]->badp;
      }
      if (Questions[actual]->deftimepset){
        timeb = TestInfo.GBtimeb;
        freetime = TestInfo.GBft;                                  
      } else {
        timeb = Questions[actual]->timebonus;
        freetime = Questions[actual]->freetime;       
      }
      for (int i = 0;i < BtnsC;i++){
        if (((TGoodorBadQ*)Questions[actual])->Good[i] == Btns[i].checked){
          SAnswersPtsAll += goodp;
          Results[actual].Positive = true;
        } else SAnswersPtsAll += badp;    
      }
      SAnswersPtsMax = goodp*BtnsC;
      SAnswersPtsMin = badp*BtnsC;
    } break;
    case QT_ESTIMATION:{
      int passrange;
      bool inaccpass;
      int range;
      int pay;
      int maxrange;
      if (Questions[actual]->defpointset){
        goodp = TestInfo.Egp;
        badp = TestInfo.Ebp;
        passrange = TestInfo.Epassrange;
        inaccpass = TestInfo.Einaccpass;
        range = TestInfo.Erange;
        pay = TestInfo.Eppay;
        maxrange = TestInfo.Emaxrange;                             
      } else {
        goodp = Questions[actual]->goodp;
        badp = Questions[actual]->badp;
        passrange = ((TEstimationQ*)Questions[actual])->passrange;
        inaccpass = ((TEstimationQ*)Questions[actual])->inaccuratepass;
        range = ((TEstimationQ*)Questions[actual])->range;
        pay = ((TEstimationQ*)Questions[actual])->ppay;
        maxrange = ((TEstimationQ*)Questions[actual])->maxrange;    
      }
      if (Questions[actual]->deftimepset){
        timeb = TestInfo.Etimeb;
        freetime = TestInfo.Eft;                                  
      } else {
        timeb = Questions[actual]->timebonus;
        freetime = Questions[actual]->freetime;       
      }
      DWORD x = GetWindowTextLength(HEdit);
      LPSTR temp = (LPSTR)GlobalAlloc(GPTR,x+1);
      GetWindowText(HEdit,temp,x+1);
      int myans = strtoint(temp);
      cout << inttostr(myans).s << "  " << inttostr(((TEstimationQ*)Questions[actual])->A).s << "\n";
      GlobalFree(temp);
      int delta = abs(myans-((TEstimationQ*)Questions[actual])->A);
      if (delta <= passrange){
        SAnswersPtsAll += goodp;
        Results[actual].Positive = true;     
      } else {
        delta -= passrange;
        if (inaccpass){
          if (delta <= maxrange){
            SAnswersPtsAll += goodp - int(pay*int(delta/range));
            Results[actual].Positive = true;
          } else SAnswersPtsAll += badp;
        } else SAnswersPtsAll += badp;     
      }
      SAnswersPtsMax += goodp;
      SAnswersPtsMin += badp;
    } break;       
  }   
  t -= freetime;
  if (Results[actual].Positive){
    SSpeedPtsAll += Min(time-t,time)*timeb;
	AnswerMark = AM_GOOD;
	ACPointsMax = goodp;
  } else {
	AnswerMark = AM_BAD;
	ACPointsMax = badp;
  }
  
  SSpeedPtsMax += time*timeb;  
  cout << inttostr(SAnswersPtsAll).s << " pts\n";
  cout << inttostr(SSpeedPtsAll).s << " pts\n";  
  cout << "~CheckAnswer\n"; 
}

void TTestForm::Draw(HDC hdc){

  TestFormGraphics* TFG = (TestFormGraphics*)GetClassLong(hwnd,0);
  
  //Background
  DrawBackground(Buffer);
  
  //Question
  HFONT OldFont = (HFONT)SelectObject(Buffer,QFont);
  SetBkMode(Buffer,TRANSPARENT);
  DrawText(Buffer,Q.s,-1,&QRect,DT_CENTER|DT_WORDBREAK);
  
  //Buttons
  for (int i = 0;i < BtnsC;i++) DrawButton(Buffer,i);

  //Timer
  if ((time != 0) && (State == S_QUESTION)) DrawTimer(Buffer);
  
  //Tip
  if (IsTip && (TipLength > -1) && (State == S_QUESTION)){
    SelectObject(Buffer,TipFont);
    DrawText(Buffer,TipStr,-1,&TipRect,DT_CENTER);          
  }
  
  //AnswerCheck
  if (State == S_ANSWERCHECK) DrawAnswerCheck(Buffer);
  
  SelectObject(Buffer,ButtonsFont);
  
  SStr S;
  strcpy(S.s,inttostr(actual+1).s);
  strcat(S.s,"/");
  strcat(S.s,inttostr(TestInfo.QuestionsC).s);
  
  RECT rc = EmptyRect();
  DrawText(Buffer,S.s,-1,&rc,DT_CALCRECT);
  rc.left += 10;
  rc.right += 10;
  int PosyDif = TESTFORMH-6-rc.bottom;
  rc.top += PosyDif;
  rc.bottom += PosyDif;
  DrawText(Buffer,S.s,-1,&rc,DT_CENTER);
  
  SelectObject(Buffer,OldFont);
  BitBlt(hdc,0,0,TESTFORMW,TESTFORMH,Buffer,0,0,SRCCOPY);

}

void TTestForm::DrawBackground(HDC hdc){

  TestFormGraphics* TFG = (TestFormGraphics*)GetClassLong(hwnd,0);

  SelectObject(Buffer2,TFG->BK);
  BitBlt(hdc,0,0,TESTFORMW,TESTFORMH,Buffer2,0,0,SRCCOPY);
  SelectObject(Buffer2,NULL);  
  	
}

void TTestForm::DrawButton(HDC hdc,int x){
  TestFormGraphics* TFG = (TestFormGraphics*)GetClassLong(hwnd,0);
  if (Btns[x].checked){
    if (Clicked == x){
      if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB6Mask);      
      else SelectObject(Buffer2,TFG->RCB6Mask);
      BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCAND);
      if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB6);
      else SelectObject(Buffer2,TFG->RCB6);
      BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCPAINT);             
    } else {
      if (MouseOn == x){
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB4Mask);
        else SelectObject(Buffer2,TFG->RCB4Mask);
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCAND);
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB4);
        else SelectObject(Buffer2,TFG->RCB4);               
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCPAINT);
      } else {
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB2Mask);
        else SelectObject(Buffer2,TFG->RCB2Mask);
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCAND);
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB2);
        else SelectObject(Buffer2,TFG->RCB2);
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCPAINT);
      }
    }
  } else {
    if (Clicked == x){
      if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB5Mask);
      else SelectObject(Buffer2,TFG->RCB5Mask);
      BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCAND);
      if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB5);
      else SelectObject(Buffer2,TFG->RCB5);
      BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCPAINT);           
    } else {
      if (MouseOn == x){
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB3Mask);
        else SelectObject(Buffer2,TFG->RCB3Mask);
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCAND);
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB3);
        else SelectObject(Buffer2,TFG->RCB3);               
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCPAINT);
      } else {
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB1Mask);
        else SelectObject(Buffer2,TFG->RCB1Mask);
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCAND);
        if (type == QT_ABCD) SelectObject(Buffer2,TFG->RB1);
        else SelectObject(Buffer2,TFG->RCB1);
        BitBlt(hdc,Btns[x].x,Btns[x].y,16,16,Buffer2,0,0,SRCPAINT);
      }
    }      
  }      
  SelectObject(Buffer2,NULL);
  SetBkMode(hdc,TRANSPARENT);
  HFONT OldFont = (HFONT)SelectObject(hdc,ButtonsFont);
  DrawText(hdc,Btns[x].text,-1,&Btns[x].TextRect,DT_WORDBREAK);
  SelectObject(hdc,OldFont);
}

void TTestForm::DrawTimer(HDC hdc){
  TestFormGraphics* TFG = (TestFormGraphics*)GetClassLong(hwnd,0);
  double t = (lt-ts)-(int((lt-ts)/.5)*.5);
  int i = int(t*24);
  SelectObject(Buffer2,TFG->CBs[i]);
  BitBlt(hdc,TIMERX,TIMERY,TIMERSIZE,TIMERSIZE,Buffer2,0,0,SRCCOPY);
  SelectObject(Buffer2,NULL);
  int timetoshow = Max(time+freetime-int(lt-ts),0); //time displayed on the screen
  if (timetoshow > time){
    SetTextColor(hdc,RGB(35,200,15));
  }
  HFONT OldFont = (HFONT)SelectObject(hdc,TimerFont);
  RECT rect;
  rect.top = TIMERY;
  rect.left = TIMERX;
  rect.right = rect.left + TIMERSIZE;
  rect.bottom = rect.top + TIMERSIZE;
  DrawText(hdc,inttostr(timetoshow).s,-1,&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
  SelectObject(hdc,OldFont);
  if (timetoshow > time){
    SetTextColor(hdc,RGB(0,0,0));
  }
}

int last,Summary,Draw,Total,Gauge,Points;

void TTestForm::DrawGauge(HDC hdc,RECT rc,double pos){
  int x1,x2;
  x1 = MemoryUsage(hwnd);  x2 = MemoryUsage(hwnd);
  HBRUSH Black = CreateSolidBrush(RGB(0,0,0));
  HBRUSH Brush;

  cout << inttostr(pos).s << "\n";
  FrameRect(hdc,&rc,Black);
  rc.top++;
  rc.left++;
  rc.right--;
  rc.bottom--;
  if (SAnswersPtsMax != 0){
    rc.right = rc.left + (rc.right-rc.left)*pos;
    int R = Max(0,200*(1 - Maxd(0,pos-0.5)));
    int G = Min(200,200*2*pos);
    Brush = CreateSolidBrush(RGB(R,G,0));
    FillRect(hdc,&rc,Brush);  
  }

  DeleteObject(Black);
  DeleteObject(Brush);
  int x3 = MemoryUsage(hwnd);

  if (x2-x1) cout << "DrawGauge\n";
  Gauge += x2-x1;
}

void TTestForm::DrawPoints(HDC hdc,RECT rc,int Points,int MaxPoints){
  int x1,x2;
  x1 = MemoryUsage(hwnd);
  cout << "In DrawPoints() :\n";
  HFONT OldFont = (HFONT) SelectObject(hdc,ACPointsFont);
  
  if (Points <= 0) SetTextColor(hdc,RGB(200,35,15));
  else SetTextColor(hdc,RGB(35,200,15));
  
  SStr String;
  strcpy(String.s,inttostr(Points).s);
  strcat(String.s," pts / ");
  strcat(String.s,inttostr(MaxPoints).s);
  strcat(String.s," pts");
  
  RECT TempRect = EmptyRect();
  int PosxDif,PosyDif;
  
  cout << TempRect << "\n";
  
  DrawText(hdc,String.s,-1,&TempRect,DT_CALCRECT);
  
  cout << TempRect << "\n";
  
  PosxDif = rc.left + (rc.right-rc.left-TempRect.right+TempRect.left)/2 - TempRect.left;
  PosyDif = rc.top + (rc.bottom-rc.top-TempRect.bottom+TempRect.top)/2 - TempRect.top;
  
  TempRect.left += PosxDif;
  TempRect.right += PosxDif;
  TempRect.top += PosyDif;
  TempRect.bottom += PosyDif;
  
  DrawText(hdc,String.s,-1,&TempRect,DT_CENTER);
  
  SelectObject(hdc,OldFont);
  SetTextColor(hdc,RGB(0,0,0));
  cout << "~DrawPoints()\n";
  x2 = MemoryUsage(hwnd);

  if (x2-x1) cout << "DrawPoints\n";
  Points += x2-x1;

}

void TTestForm::DrawCorrectionTests(HDC hdc){
	
  TestFormGraphics* TFG = (TestFormGraphics*)GetClassLong(hwnd,0);   
  
  SelectObject(Buffer2,TFG->BKA);
  BitBlt(hdc,CT_LEFT,CTPosy,CT_WIDTH,CT_HEIGHT,Buffer2,0,0,SRCCOPY);
  if (CTPosy + CT_HEIGHT > TESTFORMH-20-BUTTONH) ExitBtn->Redraw();
  if (CTPosy + CT_HEIGHT > TESTFORMH-30-BUTTONH*2) CTestsBtn->Redraw();
  
}

void TTestForm::DrawSummary(HDC hdc){
  int x1,x2,x3,x4,x5;
  
  x1 = MemoryUsage(hwnd);

  cout << "In DrawSummary() :\n";
  cout << "State = " << inttostr(State).s << "\n";
  
  RECT Ptsrc;
  Ptsrc.left = SAnswersRect.right + 10;
  Ptsrc.right = TESTFORMW - 10;
  Ptsrc.top = SAnswersRect.top;
  Ptsrc.bottom = SAnswersRect.bottom;
  
  //Background
  DrawBackground(Buffer); 
  
  x2 = MemoryUsage(hwnd);
  
  cout << "SanswersPts = " << inttostr(SAnswersPts).s << "   SAnswersPtsMax = " << inttostr(SAnswersPtsMax).s << "\n";
  
  //Answers
  DrawGauge(Buffer,SAnswersRect,double(SAnswersPts-SAnswersPtsMin)/double(SAnswersPtsMax-SAnswersPtsMin));
  DrawText(Buffer,"Answers :",-1,&SAnswersTextRect,0);
  if (SState > SS_ANSWERS1) DrawPoints(Buffer,Ptsrc,SAnswersPts,SAnswersPtsMax);
  
  x3 = MemoryUsage(hwnd);
  
  //Speed
  if (SState >= SS_SPEED1){
	DrawGauge(Buffer,SSpeedRect,double(SSpeedPts)/double(SSpeedPtsMax));
    DrawText(Buffer,"Speed :",-1,&SSpeedTextRect,0);
    Ptsrc.top += SSpeedRect.top-SAnswersRect.top;
    Ptsrc.bottom += SSpeedRect.bottom-SAnswersRect.bottom;
    if (SState > SS_SPEED1) DrawPoints(Buffer,Ptsrc,SSpeedPts,SSpeedPtsMax);
  }
  
  x4 = MemoryUsage(hwnd);
  
  //All
  if (SState >= SS_ALL1){
	DrawGauge(Buffer,SPtsRect,double(SPts-SAnswersPtsMin)/double(SPtsMax-SAnswersPtsMin));
    DrawText(Buffer,"Total :",-1,&SPtsTextRect,0);
    Ptsrc.top += SPtsRect.top - SSpeedRect.top;
    Ptsrc.bottom += SPtsRect.bottom - SSpeedRect.bottom;
    if (SState > SS_ALL1) DrawPoints(Buffer,Ptsrc,SPts,SPtsMax);
  }
  
  if (State >= S_CTANIMATION1) DrawCorrectionTests(Buffer);
  
  BitBlt(hdc,0,0,TESTFORMW,TESTFORMH,Buffer,0,0,SRCCOPY);
  
  x5 = MemoryUsage(hwnd);
  
  if (x2-x1 > 0) cout << "1-2\n";
  if (x3-x2 > 0) cout << "2-3\n";
  if (x4-x3 > 0) cout << "3-4\n";
  if (x5-x4 > 0) cout << "4-5\n";
  
  cout << "~DrawSummary()\n";
  
}

void TTestForm::DrawAnswerCheck(HDC hdc){
  cout << "In TTestForm::DrawAnswerCheck() :\n";
  
  TestFormGraphics* TFG = (TestFormGraphics*)GetClassLong(hwnd,0);   
  
  SelectObject(Buffer2,TFG->BKA);
  BitBlt(hdc,AC_LEFT,ACPosy,AC_WIDTH,AC_HEIGHT,Buffer2,0,0,SRCCOPY);
  
  if (TickCrossNum != -1){
    if (AnswerMark == AM_GOOD){
      SelectObject(Buffer2,TFG->TickMasks[TickCrossNum]);
      BitBlt(hdc,AC_LEFT+20,ACPosy+6,AC_TICKWIDTH,AC_TICKHEIGHT,Buffer2,0,0,SRCAND);
      SelectObject(Buffer2,TFG->Ticks[TickCrossNum]);
      BitBlt(hdc,AC_LEFT+20,ACPosy+6,AC_TICKWIDTH,AC_TICKHEIGHT,Buffer2,0,0,SRCPAINT);              
    } else if (AnswerMark == AM_BAD){
	  SelectObject(Buffer2,TFG->CrossMasks[TickCrossNum]);
	  BitBlt(hdc,AC_LEFT+20,ACPosy+6,AC_CROSSWIDTH,AC_CROSSHEIGHT,Buffer2,0,0,SRCAND);
	  SelectObject(Buffer2,TFG->Crosses[TickCrossNum]);
	  BitBlt(hdc,AC_LEFT+20,ACPosy+6,AC_CROSSWIDTH,AC_CROSSHEIGHT,Buffer2,0,0,SRCPAINT);
    } else if (AnswerMark == AM_MEDIOCRE) {
		
    }
  }
  
  RECT rc;
  int Ansh,PosxDif;
  HFONT OldFont;
  Ansh = 0;
  
  if (type == QT_ABCD){
    
    OldFont = (HFONT)SelectObject(hdc,ButtonsFont);
    
    SStr Answer;
    strcpy(Answer.s,"Answer : ");
    strcat(Answer.s,((TABCDQ*)Questions[actual])->As[((TABCDQ*)Questions[actual])->a].s);
    
    if (AnswerMark == AM_GOOD) rc.left = AC_LEFT+20+AC_TICKWIDTH+10;
    else if (AnswerMark == AM_BAD) rc.left = AC_LEFT+20+AC_CROSSWIDTH+10;
    else ;
    rc.right = AC_LEFT+AC_WIDTH-10;
    rc.top = ACPosy+6;
    rc.bottom = rc.top;
  
    DrawText(hdc,Answer.s,-1,&rc,DT_WORDBREAK|DT_CENTER|DT_CALCRECT);
    
    PosxDif;
    if (AnswerMark == AM_GOOD) PosxDif = (AC_WIDTH-40-AC_TICKWIDTH-rc.right+rc.left)/2;
    else if (AnswerMark == AM_BAD) PosxDif = (AC_WIDTH-40-AC_CROSSWIDTH-rc.right+rc.left)/2;
    else ;
    
    rc.left += PosxDif;
    rc.right += PosxDif;

	Ansh = rc.bottom-rc.top;
    
    DrawText(hdc,Answer.s,-1,&rc,DT_CENTER|DT_WORDBREAK);
  
  }
  
  if (type == QT_BYHAND){
    
    OldFont = (HFONT)SelectObject(hdc,ButtonsFont);
    
    SStr Answer;
    strcpy(Answer.s,"Answer : ");
    strcat(Answer.s,((TByHandQ*)Questions[actual])->A.s);
    
    if (AnswerMark == AM_GOOD) rc.left = AC_LEFT+20+AC_TICKWIDTH+10;
    else if (AnswerMark == AM_BAD) rc.left = AC_LEFT+20+AC_CROSSWIDTH+10;
    else ;
    rc.right = AC_LEFT+AC_WIDTH-10;
    rc.top = ACPosy+6;
    rc.bottom = rc.top;
  
    DrawText(hdc,Answer.s,-1,&rc,DT_WORDBREAK|DT_CENTER|DT_CALCRECT);
    
    PosxDif;
    if (AnswerMark == AM_GOOD) PosxDif = (AC_WIDTH-40-AC_TICKWIDTH-rc.right+rc.left)/2;
    else if (AnswerMark == AM_BAD) PosxDif = (AC_WIDTH-40-AC_CROSSWIDTH-rc.right+rc.left)/2;
    else ;
    
    rc.left += PosxDif;
    rc.right += PosxDif;

	Ansh = rc.bottom-rc.top;
    
    DrawText(hdc,Answer.s,-1,&rc,DT_CENTER|DT_WORDBREAK);
  
  }
  
    if (ACPoints != 0){
      
      SStr Points;
      if (ACPoints < 0){
		strcpy(Points.s,"");
		SetTextColor(hdc,RGB(200,35,15));
      } else {
		strcpy(Points.s,"+");
		SetTextColor(hdc,RGB(35,200,15));
	  }
      
      strcat(Points.s,inttostr(ACPoints).s);
      strcat(Points.s," pts");
	  
	  SelectObject(hdc,ACPointsFont);
      
      if (AnswerMark == AM_GOOD) rc.left = AC_LEFT+20+AC_TICKWIDTH+10;
      else if (AnswerMark == AM_BAD) rc.left = AC_LEFT+20+AC_CROSSWIDTH+10;
      else ;
      rc.right = AC_LEFT+AC_WIDTH-10;
      rc.top = ACPosy+6+Ansh;
      rc.bottom = rc.top;
  
      DrawText(hdc,Points.s,-1,&rc,DT_CENTER|DT_CALCRECT);
      
      if (AnswerMark == AM_GOOD) PosxDif = (AC_WIDTH-40-AC_TICKWIDTH-rc.right+rc.left)/2;
      else if (AnswerMark == AM_BAD) PosxDif = (AC_WIDTH-40-AC_CROSSWIDTH-rc.right+rc.left)/2;
      else ;
      
      int PosyDif = (AC_HEIGHT-6-Ansh-rc.bottom+rc.top)/2;
      rc.bottom += PosyDif;
      rc.top += PosyDif;
      rc.left += PosxDif;
      rc.right += PosxDif;
    
      DrawText(hdc,Points.s,-1,&rc,DT_CENTER|DT_WORDBREAK);
      
      SetTextColor(hdc,RGB(0,0,0));
    }
    
    if (Enter && (ACState == AC_WAITING) && ((lt-ts)/2 - int(int(lt-ts)/2) > 0.3)){
	  SelectObject(hdc,EnterFont);
	  if (AnswerMark == AM_GOOD) rc.left = AC_LEFT+20+AC_TICKWIDTH+10;
      else if (AnswerMark == AM_BAD) PosxDif = rc.left = AC_LEFT+20+AC_CROSSWIDTH+10;
      else ;
      rc.right = AC_LEFT+AC_WIDTH-15;
      rc.top = 0;
      rc.bottom = 0;
	  DrawText(hdc,"Click or press Enter to continue ...",-1,&rc,DT_CALCRECT);
	  int PosyDif = ACPosy+AC_HEIGHT-3-rc.bottom;
	  rc.top += PosyDif;
	  rc.bottom += PosyDif;
	  PosxDif = AC_LEFT+AC_WIDTH-15-rc.right;
	  rc.left += PosxDif;
	  rc.right += PosxDif;
	  if (EnterRgn == NULL) EnterRgn = CreateRectRgnIndirect(&rc);
	  if (InEnter) SetTextColor(hdc,RGB(250,130,20));
	  DrawText(hdc,"Click or press Enter to continue ...",-1,&rc,DT_CENTER);
	  if (InEnter) SetTextColor(hdc,RGB(0,0,0));
	}
	
	SelectObject(hdc,OldFont);
   
  SelectObject(Buffer2,NULL); 
}

bool TTestForm::UpdateSummary(){
	
  SState = 0;
  
  SPtsAll = SAnswersPtsAll + SSpeedPtsAll;
  SPtsMax = SAnswersPtsMax + SSpeedPtsMax;
  
  double pos = (lt-ts)/double(SANIMATIONTIME)*6.0;
  
  SAnswersRect.left = (-POINTS_WIDTH)*Maxd(1.0-pos,0) + ANSWERSPTS_LEFT*Mind(pos,1.0);
  SAnswersTextRect.left = SAnswersRect.left;
  SAnswersRect.top = ANSWERSPTS_TOP;
  SAnswersTextRect.top = SAnswersRect.top - 20;
  SAnswersRect.right = SAnswersRect.left + POINTS_WIDTH;
  SAnswersTextRect.right = SAnswersRect.right;
  SAnswersRect.bottom = SAnswersRect.top + POINTS_HEIGHT;
  SAnswersTextRect.bottom = SAnswersRect.top - 3;
  pos--;
    
  if (pos > 0) SState++;
  SAnswersPts = Maxd(0.0,Mind(1.0,pos))*SAnswersPtsAll;
  cout << "SAnswersPts = " << inttostr(SAnswersPts).s << "\n";
  pos--;
  
  if (pos > 0) SState++;
  SSpeedRect.left = (-POINTS_WIDTH)*Maxd(1.0-pos,0.0) + SPEEDPTS_LEFT*Mind(pos,1.0);
  SSpeedTextRect.left = SSpeedRect.left;
  SSpeedRect.top = SPEEDPTS_TOP;
  SSpeedTextRect.top = SSpeedRect.top - 20;
  SSpeedRect.right = SSpeedRect.left + POINTS_WIDTH;
  SSpeedTextRect.right = SSpeedRect.right;
  SSpeedRect.bottom = SSpeedRect.top + POINTS_HEIGHT;
  SSpeedTextRect.bottom = SSpeedRect.top - 3;
  pos--;  
  
  if (pos > 0) SState++;
  SSpeedPts = Maxd(0.0,Mind(1.0,pos))*SSpeedPtsAll;
  pos--;  
  
  if (pos > 0) SState++;
  SPtsRect.left = (-POINTS_WIDTH)*Maxd(1.0-pos,0) + ALLPTS_LEFT*Mind(pos,1.0);
  SPtsTextRect.left = SPtsRect.left;
  SPtsRect.top = ALLPTS_TOP;
  SPtsTextRect.top = SPtsRect.top - 20;
  SPtsRect.right = SPtsRect.left + POINTS_WIDTH;
  SPtsTextRect.right = SPtsRect.right;
  SPtsRect.bottom = SPtsRect.top + POINTS_HEIGHT;
  SPtsTextRect.bottom = SPtsRect.top - 3;
  pos--;  
  
  if (pos > 0) SState++;
  SPts = Maxd(0.0,Mind(1.0,pos))*SPtsAll;
  pos--;     

  if (pos >= 0) return true;
  else return false;
}

double absd(double x){
  if (x < 0) return -x;
  else return x;       
}

bool TTestForm::UpdateAnswerCheck(){
  if (ACState == AC_SHOWING){
    ACPosy = TESTFORMW-int((lt-ts)/double(ACANIMATIONTIME)*(TESTFORMW-AC_TOP));
    if (ACPosy > TESTFORMW-AC_HEIGHT){
	  TickCrossNum = -1;
	  ACPoints = 0;
    } else {
	  TickCrossNum = Min(13,int((TESTFORMW-AC_HEIGHT-ACPosy)/double(TESTFORMW-AC_HEIGHT-AC_TOP)*14.0));
	  ACPoints = int(ACPointsMax*Mind(1.0,double(TESTFORMW-AC_HEIGHT-ACPosy)/double(TESTFORMW-AC_HEIGHT-AC_TOP)));
	}
    if (lt-ts >= ACANIMATIONTIME) return true;
    else return false;         
  } else if (ACState == AC_HIDING){
    TickCrossNum = 13;
    ACPosy = AC_TOP - int((lt-ts)/double(ACANIMATIONTIME)*(AC_TOP+AC_HEIGHT));
    if (lt-ts >= ACANIMATIONTIME) return true;
    else return false;    
  } else {
    if (lt-ts > ACWAITINGTIME) return true;
    else return false;       
  }  
}

bool TTestForm::UpdateCT1(){
	
  CoutOn();

  double pos = Mind(1.0,Maxd(0.0,(lt-ts)/(CTANIMATIONTIME)));
  
  CTPosy = TESTFORMW-pos*(TESTFORMW-CT_TOP);
  
  PositiveBtn->Move(CT_LEFT+20,CTPosy+5);
  NegativeBtn->Move(CT_LEFT+CT_WIDTH-20-BIGBUTTONW,CTPosy+5);
  AllBtn->Move(CT_LEFT+20,CTPosy+CT_HEIGHT-5-BIGBUTTONH);
  ChooseBtn->Move(CT_LEFT+CT_WIDTH-20-BIGBUTTONW,CTPosy+CT_HEIGHT-5-BIGBUTTONH);
  
  CoutOff();
  
  if (pos == 1.0) return true;
  else return false;
	
}

bool TTestForm::UpdateCT2(){

  double pos = Mind(1.0,Maxd(0.0,(lt-ts)/(CTANIMATIONTIME)));
  
  CTPosy = CT_TOP - pos*(CT_TOP+CT_HEIGHT);
  
  PositiveBtn->Move(CT_LEFT+20,CTPosy+5);
  NegativeBtn->Move(CT_LEFT+CT_WIDTH-20-BIGBUTTONW,CTPosy+5);
  AllBtn->Move(CT_LEFT+20,CTPosy+CT_HEIGHT-5-BIGBUTTONH);
  ChooseBtn->Move(CT_LEFT+CT_WIDTH-20-BIGBUTTONW,CTPosy+CT_HEIGHT-5-BIGBUTTONH);
  
  if (pos == 1.0) return true;
  else return false;

}

bool RegisterTestForm(HINSTANCE HInst){
     
  if (Registered) return false;
     
  bool res = true;
     
  WNDCLASSEX Window; 

  Window.hInstance = HInst;
  Window.lpszClassName = "TestForm";
  Window.lpfnWndProc = TestFormProc; 
  Window.style = 0;
  Window.cbSize = sizeof (WNDCLASSEX);
  Window.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  Window.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
  Window.hCursor = LoadCursor (NULL, IDC_ARROW);
  Window.lpszMenuName = NULL; 
  Window.cbClsExtra = 8;    
  Window.cbWndExtra = 8;      
  Window.hbrBackground = CreateSolidBrush(RGB(236,233,216));  
  
  res = res && RegisterClassEx(&Window);
  
  TestFormGraphics* TFG = new TestFormGraphics;
  
  TFG->BK = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_BK),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->BKA = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_BKA),IMAGE_BITMAP,0,0,LR_SHARED);
  for (int i = 0;i < 12;i++){
    TFG->CBs[i] = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(i+BM_CB1),IMAGE_BITMAP,0,0,LR_SHARED);    
  }
  for (int i = 0;i < 14;i++){
    TFG->Ticks[i] = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_TICK1+i),IMAGE_BITMAP,0,0,LR_SHARED);
    TFG->TickMasks[i] = CreateBitmapMask(TFG->Ticks[i],RGB(222,222,221));    
  }
  for (int i = 0;i < 14;i++){
    TFG->Crosses[i] = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_CROSS1+i),IMAGE_BITMAP,0,0,LR_SHARED);
	TFG->CrossMasks[i] = CreateBitmapMask(TFG->Crosses[i],RGB(225,226,225));		
  }
  TFG->RB1 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RB1),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RB2 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RB2),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RB3 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RB3),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RB4 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RB4),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RB5 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RB5),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RB6 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RB6),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RB1Mask = CreateBitmapMask(TFG->RB1,RGB(192,192,192));
  TFG->RB2Mask = CreateBitmapMask(TFG->RB2,RGB(192,192,192));
  TFG->RB3Mask = CreateBitmapMask(TFG->RB3,RGB(192,192,192));
  TFG->RB4Mask = CreateBitmapMask(TFG->RB4,RGB(192,192,192));
  TFG->RB5Mask = CreateBitmapMask(TFG->RB5,RGB(192,192,192));
  TFG->RB6Mask = CreateBitmapMask(TFG->RB6,RGB(192,192,192));
  
  TFG->RCB1 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RCB1),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RCB2 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RCB2),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RCB3 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RCB3),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RCB4 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RCB4),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RCB5 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RCB5),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RCB6 = (HBITMAP)LoadImage(HInst,MAKEINTRESOURCE(BM_RCB6),IMAGE_BITMAP,0,0,LR_SHARED);
  TFG->RCB1Mask = CreateBitmapMask(TFG->RCB1,RGB(192,192,192));
  TFG->RCB2Mask = CreateBitmapMask(TFG->RCB2,RGB(192,192,192));
  TFG->RCB3Mask = CreateBitmapMask(TFG->RCB3,RGB(192,192,192));
  TFG->RCB4Mask = CreateBitmapMask(TFG->RCB4,RGB(192,192,192));
  TFG->RCB5Mask = CreateBitmapMask(TFG->RCB5,RGB(192,192,192));
  TFG->RCB6Mask = CreateBitmapMask(TFG->RCB6,RGB(192,192,192));
  
  HWND Temp = CreateWindow("TestForm","",0,0,0,0,0,NULL,NULL,HInst,NULL);
  res = res && (Temp != NULL);
  
  SetClassLong(Temp,0,(LONG)TFG);
  
  srand(time(NULL));
  
  Registered = res;
  return res;
    
}

LRESULT CALLBACK TestFormProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
  switch (message){
    case WM_TIMER:{
      TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
      if (TF != NULL){
        switch (TF->State){
          case S_ANIMATION1:{
            TF->lt = TF->GetSecs();
            if (TF->UpdatePositions1()){
              TF->State = S_QUESTION;
              if (TF->type == QT_BYHAND || TF->type == QT_ESTIMATION) SetFocus(TF->HEdit);
              TF->ts = TF->GetSecs();
              TF->SetRegions();
            }
            HDC hdc = GetDC(hwnd);
            TF->Draw(hdc);
            if (TF->type == QT_BYHAND || TF->type == QT_ESTIMATION) RedrawWindow(TF->HEdit,NULL,NULL,RDW_INVALIDATE);
            ReleaseDC(hwnd,hdc);  
          } break;
          case S_QUESTION:{
            TF->lt = TF->GetSecs();
            if ((TF->time != 0) && (TF->lt - TF->ts > TF->time+TF->freetime)){
              TF->CheckAnswer();
              if (TF->ShowAnswer){
                 TF->State = S_ANSWERCHECK;
                 TF->ACState = AC_SHOWING;
                 ShowWindow(TF->HEdit,SW_HIDE);        
              } else {
                TF->State = S_ANIMATION2;  
              }
              TF->ts = TF->GetSecs();          
            } 
            if (TF->IsTip && (TF->lt-TF->ts-TF->TipStart-TF->freetime >= 0)){
              
              int shouldtip = Min(TF->AnsLength,int(double(TF->lt-TF->ts-TF->TipStart-TF->freetime)/double(TF->time-TF->TipStart)*double(TF->TipMax+1)));
              
              while (TF->TipLength < shouldtip){
                    
                if (TF->TipLength > -1){
                  int i = rand()%(TF->AnsLength-TF->TipLength);
                  for (int j = 0;j < TF->AnsLength;j++){
                    if (!TF->Tip[j]){
                      if (!i){
                        TF->Tip[j] = true;
                        break;    
                      }
                      i--;             
                    }    
                  }
                } 
                TF->TipLength++;
                TF->TipStr = TF->GenerateTip(((TByHandQ*)TF->Questions[TF->actual])->A.s);    
              }
            }
            HDC hdc = GetDC(hwnd);
            TF->Draw(hdc);
            RedrawWindow(TF->HEdit,NULL,NULL,RDW_INVALIDATE);
            ReleaseDC(hwnd,hdc); 
          } break;
          case S_ANSWERCHECK:{
            TF->lt = TF->GetSecs();
            if (TF->UpdateAnswerCheck()){
              if (TF->ACState == AC_HIDING){
                TF->State = S_ANIMATION2;  
				TF->ts = TF->GetSecs();       
              } else if (TF->ACState == AC_SHOWING){
				TF->ACState++;
				TF->ts = TF->GetSecs();
				TF->InEnter = false; 
              } else if (!TF->Enter) {
			    TF->ACState++;
			    TF->ts = TF->GetSecs();
			  }
                
            }
            HDC hdc = GetDC(hwnd);
            TF->Draw(hdc);
            ReleaseDC(hwnd,hdc);
          } break;
          case S_ANIMATION2:{
            TF->lt = TF->GetSecs();
            if (TF->UpdatePositions2()){
              if (TF->TestInfo.QuestionsC > TF->actual+1){
                TF->SetQuestion(TF->actual + 1);
                TF->ts = TF->GetSecs();
                TF->State = S_ANIMATION1;                        
              } else {
                TF->ts = TF->GetSecs();
                
                TF->State = S_SANIMATION;       
              }
            }
            HDC hdc = GetDC(hwnd);
            TF->Draw(hdc);
            RedrawWindow(TF->HEdit,NULL,NULL,RDW_INVALIDATE);
            ReleaseDC(hwnd,hdc);
          } break;
          case S_SANIMATION:{
            TF->lt = TF->GetSecs();
            if (TF->UpdateSummary()){
              TF->State = S_SUMMARY;
              TF->ExitBtn->Show();
              if (TF->CorrectionTests) TF->CTestsBtn->Show();
			  TF->ts = TF->GetSecs();
			  last = MemoryUsage(hwnd);                        
            }
            HDC hdc = GetDC(hwnd);
            TF->DrawSummary(hdc);
            ReleaseDC(hwnd,hdc);    
          } break;
          case S_SUMMARY:{
			int x1 = MemoryUsage(hwnd);
			
			TF->lt = TF->GetSecs();
            HDC hdc = GetDC(hwnd);
            int x2 = MemoryUsage(hwnd);
            TF->DrawSummary(hdc);
            int x3 = MemoryUsage(hwnd);
            ReleaseDC(hwnd,hdc);
          } break;
		  case S_CTANIMATION1:{
			cout << "S_CTANIMATION1\n";
			TF->lt = TF->GetSecs();
			if (TF->UpdateCT1()){
			  cout << "Next State : S_CTWAITING\n";
			  TF->State = S_CTWAITING;
			  TF->ts = TF->GetSecs();	
			}
            HDC hdc = GetDC(hwnd);
            TF->DrawSummary(hdc);
            ReleaseDC(hwnd,hdc);
            cout << "State = " << inttostr(TF->State).s << "\n";
            cout << "~S_CTANIMATION1\n";
		  } break;
		  case S_CTWAITING:{
			cout << "S_CTWAITING\n";
			TF->lt = TF->GetSecs();
            HDC hdc = GetDC(hwnd);
            TF->DrawSummary(hdc);
            ReleaseDC(hwnd,hdc);
		  } break;
		  case S_CTANIMATION2:{
			TF->lt = TF->GetSecs();
			if (TF->UpdateCT2()){
			  switch (TF->CTOption){
			    case CT_POSITIVE:{
				  KillTimer(hwnd,1);
				  TF->ExitBtn->Hide();
				  TF->CTestsBtn->Hide();
				  TF->PositiveBtn->Hide();
				  TF->NegativeBtn->Hide();
				  TF->AllBtn->Hide();
				  TF->ChooseBtn->Hide();
				  TF->StartTest();
				} break; case CT_NEGATIVE:{
				  KillTimer(hwnd,1);
				  TF->ExitBtn->Hide();
				  TF->CTestsBtn->Hide();
				  TF->PositiveBtn->Hide();
				  TF->NegativeBtn->Hide();
				  TF->AllBtn->Hide();
				  TF->ChooseBtn->Hide();
				  TF->StartTest();
				} break; case CT_ALL:{
				  KillTimer(hwnd,1);
				  TF->ExitBtn->Hide();
				  TF->CTestsBtn->Hide();
				  TF->PositiveBtn->Hide();
				  TF->NegativeBtn->Hide();
				  TF->AllBtn->Hide();
				  TF->ChooseBtn->Hide();
				  TF->StartTest();
				} break; case CT_CHOOSE:{
				  KillTimer(hwnd,1);
				  TF->ExitBtn->Hide();
				  TF->CTestsBtn->Hide();
				  TF->PositiveBtn->Hide();
				  TF->NegativeBtn->Hide();
				  TF->AllBtn->Hide();
				  TF->ChooseBtn->Hide();
				  RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE);
				  TF->State = S_NOTHING;
				} break; default :{
				  TF->ts = TF->GetSecs();
				  TF->State = S_SUMMARY;	
				}		
			  }	
			}
            HDC hdc = GetDC(hwnd);
            TF->DrawSummary(hdc);
            ReleaseDC(hwnd,hdc); 		    		
		  }
        }
      }
    } break;
    case WM_LBUTTONDOWN:{
      TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
      if (TF != NULL){   
        if ((TF->State == S_QUESTION) && ((TF->type == QT_ABCD) || (TF->type == QT_GOODORBAD))){
          HDC hdc = GetDC(hwnd);
          if (TF->MouseOn != -1){
            SetCapture(hwnd);
            TF->Clicked = TF->MouseOn;
            TF->DrawButton(hdc,TF->MouseOn);                
          }
          ReleaseDC(hwnd,hdc);
        } else if ((TF->State == S_ANSWERCHECK) && (TF->ACState == AC_WAITING) && (TF->EnterRgn != NULL) && (TF->InEnter)){
		  	TF->ACState++;
			TF->ts = TF->GetSecs();	
		}
      }   
    } break;
    case WM_LBUTTONUP:{
      TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
      if (TF != NULL){
        if ((TF->State == S_QUESTION) && ((TF->type == QT_ABCD) || (TF->type == QT_GOODORBAD))){
        ReleaseCapture();
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        HDC hdc = GetDC(hwnd);
        if (PtInRegion(TF->Btns[TF->Clicked].HRegion,x,y)){
          
          if (TF->Btns[TF->Clicked].checked){
            TF->Btns[TF->Clicked].checked = false;                                   
          } else {
            TF->Btns[TF->Clicked].checked = true;
            if (TF->type == QT_ABCD){
              if (TF->InstantAnswering){
                TF->CheckAnswer(); 
				if (TF->ShowAnswer){
                  TF->State = S_ANSWERCHECK;
                  TF->ACState = AC_SHOWING;
                  ShowWindow(TF->HEdit,SW_HIDE);        
                } else {
                  TF->State = S_ANIMATION2;  
                }
              TF->ts = TF->GetSecs();                     
              } else {
                for (int i = 0;i < TF->BtnsC;i++) if (i != TF->Clicked){
                  TF->Btns[i].checked = false;
                  TF->DrawButton(hdc,i);
                }         
              }      
            }
          }
        
        }
        int temp = TF->Clicked;
        TF->Clicked = -1;
        if (temp != -1) TF->DrawButton(hdc,temp);
        ReleaseDC(hwnd,hdc);
        }
      }
    } break;
    case WM_MOUSEMOVE:{
      TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
      if (TF != NULL){
        if ((TF->State == S_QUESTION) && ((TF->type == QT_ABCD) || (TF->type == QT_GOODORBAD))){
          HDC hdc = GetDC(hwnd);
          int x = LOWORD(lParam);
          int y = HIWORD(lParam);
          int i = 0;
          for (i = 0;i < TF->BtnsC;i++) if (PtInRegion(TF->Btns[i].HRegion,x,y)) break;
          if (i == TF->BtnsC) i = -1;
          if (TF->MouseOn != i){
            int old = TF->MouseOn;
            TF->MouseOn = i;
            if (i != -1) TF->DrawButton(hdc,i);
            if (old != -1) TF->DrawButton(hdc,old);        
          }
          ReleaseDC(hwnd,hdc); 
        } else if ((TF->State == S_ANSWERCHECK) && (TF->ACState == AC_WAITING) && (TF->EnterRgn != NULL)){
		  int x = LOWORD(lParam);
		  int y = HIWORD(lParam);
		  if (PtInRegion(TF->EnterRgn,x,y)) TF->InEnter = true;
		  else TF->InEnter = false;
		}
      }   
    } break;
    case WM_PAINT:{
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd,&ps);
      TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
      if (TF != NULL){
	    switch (TF->State){
		  case S_NOTHING:{
			TF->DrawBackground(hdc);
		  } break;
		  case S_ANIMATION1:
		  case S_QUESTION:
		  case S_ANSWERCHECK:
		  case S_ANIMATION2:{
			TF->Draw(hdc);	
		  } break;
		  case S_SANIMATION:
		  case S_SUMMARY:
		  case S_CTANIMATION1:
		  case S_CTWAITING:
		  case S_CTANIMATION2:{
			TF->DrawSummary(hdc);	
		  } break;
		  default : TF->DrawBackground(hdc);	
		}       
      }
      EndPaint(hwnd,&ps);   
    } break;
    case WM_KEYDOWN:{
	  CoutOn();
	  CoutOff();
      TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
      if (TF != NULL){
        if (wParam == VK_RETURN){
		  if ((TF->State == S_QUESTION) && (TF->type == QT_ESTIMATION  || TF->type == QT_BYHAND)){
            TF->CheckAnswer();
            if (TF->ShowAnswer){
              TF->State = S_ANSWERCHECK;
              TF->ACState = AC_SHOWING;
              ShowWindow(TF->HEdit,SW_HIDE);        
            } else {
              TF->State = S_ANIMATION2;  
            }
              TF->ts = TF->GetSecs();
		  } else if ((TF->State == S_ANSWERCHECK) && (TF->ACState == AC_WAITING) && (TF->Enter)){
		    TF->ACState++;
			TF->ts = TF->GetSecs();		
		  }         
        } 
      }
    } break;
    case WM_COMMAND:{
	  TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
	  if (TF != NULL){
		switch (LOWORD(wParam)){
		  case ID_EXITBTN:
			TF->State = S_NOTHING;
			RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE);
			TF->ExitBtn->Hide();
			TF->CTestsBtn->Hide();
			KillTimer(hwnd,1);
			SendMessage(TF->Parent,WM_USER+MWM_TESTEND,0,0);
			cout << "ExitBtn clicked\n";
			break;
		  case ID_CTESTSBTN:{
			if (TF->State == S_SUMMARY){
			  TF->State = S_CTANIMATION1;
			  TF->NegativeBtn->Show();
			  TF->PositiveBtn->Show();
			  TF->AllBtn->Show();
			  TF->ChooseBtn->Show();
			  TF->CTOption = -1;
			  TF->ts = TF->GetSecs();
		    } else if (TF->State == S_CTWAITING){
			  TF->State = S_CTANIMATION2;
			  TF->ts = TF->GetSecs();
			}
			cout << "CTestsBtn clicked\n";
			break;	
		  } case ID_POSITIVEBTN:{
			int count = 0;
			for (int i = 0;i < TF->TestInfo.QuestionsC;++i){
			  if (TF->Results[i].Positive)	++count;
			}
			PQuestion *Q;
			Q = new PQuestion[count];
			int j = 0;
			for (int i = 0;i < TF->TestInfo.QuestionsC;++i){
			  if (TF->Results[i].Positive){
				switch (TF->Questions[i]->type){
				  case QT_ABCD:{
				    Q[j] = new TABCDQ;
				    *((TABCDQ*)(Q[j++])) = *((TABCDQ*)(TF->Questions[i]));
				  } break; case QT_BYHAND:{
				    Q[j] = new TByHandQ;
				    *((TByHandQ*)(Q[j++])) = *((TByHandQ*)(TF->Questions[i]));		
				  } break; case QT_GOODORBAD:{
				    Q[j] = new TGoodorBadQ;
				    *((TGoodorBadQ*)(Q[j++])) = *((TGoodorBadQ*)(TF->Questions[i]));
				  } break; case QT_ESTIMATION:{
				    Q[j] = new TEstimationQ;
				    *((TEstimationQ*)(Q[j++])) = *((TEstimationQ*)(TF->Questions[i]));	
				  }
			    }
			  }
			}
			TF->Questions = Q;
			TF->TestInfo.QuestionsC = count;
			TF->OwnQs = true;
			TF->CTOption = CT_POSITIVE;
			TF->State = S_CTANIMATION2;
			TF->ts = TF->GetSecs();
		  } break; case ID_NEGATIVEBTN:{
			int count = 0;
			for (int i = 0;i < TF->TestInfo.QuestionsC;++i){
			  if (!TF->Results[i].Positive)	++count;
			}
			PQuestion *Q;
			Q = new PQuestion[count];
			int j = 0;
			for (int i = 0;i < TF->TestInfo.QuestionsC;++i){
			  if (!TF->Results[i].Positive){
				switch (TF->Questions[i]->type){
				  case QT_ABCD:{
				    Q[j] = new TABCDQ;
				    *((TABCDQ*)(Q[j++])) = *((TABCDQ*)(TF->Questions[i]));
				  } break; case QT_BYHAND:{
				    Q[j] = new TByHandQ;
				    *((TByHandQ*)(Q[j++])) = *((TByHandQ*)(TF->Questions[i]));		
				  } break; case QT_GOODORBAD:{
				    Q[j] = new TGoodorBadQ;
				    *((TGoodorBadQ*)(Q[j++])) = *((TGoodorBadQ*)(TF->Questions[i]));
				  } break; case QT_ESTIMATION:{
				    Q[j] = new TEstimationQ;
				    *((TEstimationQ*)(Q[j++])) = *((TEstimationQ*)(TF->Questions[i]));	
				  }
			    }
			  }
			}
			TF->Questions = Q;
			TF->TestInfo.QuestionsC = count;
			TF->CTOption = CT_NEGATIVE;
			TF->State = S_CTANIMATION2;
			TF->ts = TF->GetSecs();
			TF->OwnQs = true;
		  } break; case ID_ALLBTN:
			TF->CTOption = CT_ALL;
			TF->State = S_CTANIMATION2;
			TF->ts = TF->GetSecs();
		  break; case ID_CHOOSEBTN:
			TF->CTOption = -1;
			TF->State = S_CTANIMATION2;
			TF->ts = TF->GetSecs();	
		  break;
		}	
	  }
	 } break;
    case WM_DESTROY:{
      TTestForm* TF = (TTestForm*)GetWindowLong(hwnd,0);
      if (TF != NULL){
        SendMessage(TF->Parent,WM_USER+MWM_DESTROY,0,0);
      }   
      KillTimer(hwnd,1);
    } break;
    default : {
      return DefWindowProc(hwnd,message,wParam,lParam);       
    }
  } 
  return 0;
}

LRESULT CALLBACK MyEditProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
  WNDPROC Procedure = (WNDPROC)GetWindowLong(hwnd,GWL_USERDATA);
  switch (message){
    case WM_KEYDOWN:{
      SendMessage(GetParent(hwnd),message,wParam,lParam);    
    } break;
    case WM_CHAR:{
      if (wParam != VK_RETURN) return CallWindowProc(Procedure,hwnd,message,wParam,lParam);
    } break;
    default : return CallWindowProc(Procedure,hwnd,message,wParam,lParam);      
  }
  
  return 0;
}



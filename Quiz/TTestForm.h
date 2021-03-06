#include "Questions.h"
#include "ClientButton.h"

#define MAX_QUESTIONS 100

//My Window Messages

#define MWM_DESTROY 0 //Window abruptly closed
#define MWM_TESTEND 1

#define TESTFORMW 640    /* TestForm Size */
#define TESTFORMH 603

struct TestFormGraphics{
  HBITMAP CBs[12],Ticks[14],TickMasks[14],Crosses[14],CrossMasks[14],BK,BKA;
  HBITMAP RB1,RB2,RB3,RB4,RB5,RB6,RB1Mask,RB2Mask,RB3Mask,RB4Mask,RB5Mask,RB6Mask;
  HBITMAP RCB1,RCB2,RCB3,RCB4,RCB5,RCB6,RCB1Mask,RCB2Mask,RCB3Mask,RCB4Mask,RCB5Mask,RCB6Mask;      
};

struct TTestButton{
  int fx,fy,sx,sy;
  int x,y;
  bool checked;
  char* text;
  RECT TextRect;
  RECT FTextRect;
  RECT STextRect;    
  HRGN HRegion; 
};

struct TResult{
  int Answer;
  int Speed;
  bool Positive;
};

class TTestForm{
public :
  TTestForm(HINSTANCE HInst,HWND Parent,int x,int y);
  void Show();
  void Hide();
  bool PrepareTest(TTestInfo TestInfo,PQuestion* Qs);
  bool StartTest();
  friend LRESULT CALLBACK TestFormProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
  void DrawBackground(HDC hdc);     
private :
  int AnimationSpeed;
  int SoundVolume;
  bool Muted;
  bool InstantAnswering;
  bool ShowAnswer;
  bool ShuffleQ;
  bool ShuffleA;
  bool Enter;
  bool CorrectionTests;
  
  TTestInfo TestInfo;
  PQuestion* Questions;
  int actual;
  TResult Results[MAX_QUESTIONS];
  bool OwnQs;
  
  
  HWND hwnd, HEdit;
  HWND Parent;
  
  HFONT HEditFont;
  
  HDC Buffer,Buffer2;
  HBITMAP BufferHbm,HbmOldBuffer,HbmOldBuffer2;
  
  int State;
  double ts,lt;
  
  int type;
  
  int time;
  int freetime;
  HFONT TimerFont;
  
  RECT QFRect;
  RECT QSRect;
  RECT QRect;
  HFONT QFont;
  SStr Q;
  
  bool IsTip;
  HFONT TipFont;
  RECT TipRect;
  int TipStart;
  bool Tip[50];
  char* TipStr;
  int AnsLength;
  int TipLength; // -1 at the beginning, 0 for only number of letters
  int TipMax;
  
  bool IsEdit;
  int editfx,editsx;
  int editfy,editsy;
  
  TTestButton Btns[20];
  HFONT ButtonsFont;
  int BtnsC;
  int Clicked;
  int MouseOn;
  
  int SState;
  int SAnswersPts, SAnswersPtsAll, SAnswersPtsMax, SAnswersPtsMin;
  int SSpeedPts, SSpeedPtsAll, SSpeedPtsMax;
  int SPts, SPtsAll, SPtsMax;
  RECT SAnswersRect,SSpeedRect,SPtsRect;
  RECT SAnswersTextRect,SSpeedTextRect,SPtsTextRect;
  TClientButton *ExitBtn,*CTestsBtn;
  TClientButton *PositiveBtn,*NegativeBtn,*AllBtn,*ChooseBtn;
  
  int AnswerMark;
  
  int ACState;
  int ACPosy;
  int ACPoints;
  int ACPointsMax;
  int TickCrossNum;
  HFONT ACPointsFont;
  HFONT EnterFont;
  HRGN EnterRgn;
  bool InEnter; //mouse is in the EnterRgn
  
  int CTPosy;
  int CTOption;
  
  LARGE_INTEGER Freq;
  
  double GetSecs();
  
  char* GenerateTip(char* Answer);
  
  void SetQuestion(int q);
  void SetPositions();
  bool UpdatePositions1();
  void SetRegions();
  bool UpdatePositions2();
  void CheckAnswer();
  bool UpdateSummary();
  bool UpdateAnswerCheck();
  bool UpdateCT1();
  bool UpdateCT2();
  
  void Draw(HDC hdc);
  void DrawButton(HDC hdc,int x);
  void DrawTimer(HDC hdc);
  void DrawSummary(HDC hdc);
  void DrawGauge(HDC hdc,RECT rc,double pos); //pos (0-1)
  void DrawPoints(HDC hdc,RECT rc,int Points,int MaxPoints);
  void DrawCorrectionTests(HDC hdc);
  void DrawAnswerCheck(HDC hdc);
  
};

bool RegisterTestForm(HINSTANCE HInst);
LRESULT CALLBACK TestFormProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MyEditProc(HWND hwnd, UINT message, 9:07:64 - 21.07.12              
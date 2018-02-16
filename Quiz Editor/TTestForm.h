
#define MAX_QUESTIONS 100

#define QT_ABCD 0
#define QT_BYHAND 1
#define QT_GOODORBAD 2
#define QT_ESTIMATION 3

#define SF_SUCCESS 0
#define SF_NOTOPENED 1
#define SF_CANTWRITE 2

#define LF_SUCCESS 0
#define LF_NOTOPENED 1
#define LF_CANTREAD 2

struct TestFormGraphics{
  HBITMAP CBs[12],BK;
  HBITMAP RB1,RB2,RB3,RB4,RB5,RB6,RB1Mask,RB2Mask,RB3Mask,RB4Mask,RB5Mask,RB6Mask;
  HBITMAP RCB1,RCB2,RCB3,RCB4,RCB5,RCB6,RCB1Mask,RCB2Mask,RCB3Mask,RCB4Mask,RCB5Mask,RCB6Mask;      
};

struct SStr{
  char s[80];       
};

class TQuestion{
public :
  TQuestion();
  int type;
  SStr Q;
  bool defpointset;
  bool deftimeset;
  bool deftimepset;
  int goodp;
  int badp;
  int time;
  int timebonus;
  int freetime;
};

typedef TQuestion* PQuestion;

class TABCDQ : public TQuestion{
public :
  TABCDQ();
  int AnswersC;
  SStr As[20];
  int a;  
};

class TByHandQ : public TQuestion{
public :
  TByHandQ();
  bool deftipset;
  bool IsTip;
  int TipStart;
  int TipCost;
  SStr A;
};

class TGoodorBadQ : public TQuestion{
public :
  TGoodorBadQ(); 
  int AnswersC;     
  SStr As[20];
  bool Good[20];
};

class TEstimationQ : public TQuestion{
public : 
  TEstimationQ();
  int A;
  int passrange;
  bool inaccuratepass;
  int range;
  int ppay;
  int maxrange;      
};

struct TTestInfo{
  int QuestionsC;
  int ABCDgp,ABCDbp,ABCDtime,ABCDtimeb,ABCDft;
  int BHgp,BHbp,BHtime,BHtimeb,BHft;
  int GBgp,GBbp,GBtime,GBtimeb,GBft;
  int Egp,Ebp,Etime,Etimeb,Eft;
  bool BHtip;
  int BHtipstart;
  double BHtipcost;
  int Epassrange;
  bool Einaccpass;
  int Erange;
  int Eppay;
  int Emaxrange;
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

int SaveToFile(PQuestion *Qs,TTestInfo TestInfo,char* FileName);
int LoadFromFile(char* FileName,TTestInfo *TestInfo,PQuestion *Qs);
TTestInfo DefTestInfo();

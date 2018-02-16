#ifndef QUESTIONS_H
#define QUESTIONS_H

#define QT_ABCD 0        /* Questions' type */
#define QT_BYHAND 1
#define QT_GOODORBAD 2
#define QT_ESTIMATION 3

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

TTestInfo DefTestInfo();
bool SaveToFile(PQuestion *Qs,TTestInfo TestInfo,char* FileName);
bool LoadFromFile(char* FileName,TTestInfo *TestInfo,PQuestion *Qs);

#endif

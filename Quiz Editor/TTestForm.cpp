#include <windows.h>
#include <iostream>
#include "TTestForm.h"
using namespace std;

TQuestion::TQuestion(){
  defpointset = true;                       
}

TABCDQ::TABCDQ(){
  type = QT_ABCD;                 
}

TByHandQ::TByHandQ(){
  type = QT_BYHAND;           
}

TGoodorBadQ::TGoodorBadQ(){
  type = QT_GOODORBAD;                           
}

TEstimationQ::TEstimationQ(){
  type = QT_ESTIMATION;                             
}

int SaveToFile(PQuestion *Qs,TTestInfo TestInfo,char* FileName){
  
  DWORD Saved;
  HANDLE HFile;
  
  HFile = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  
  if (HFile == INVALID_HANDLE_VALUE){
    return SF_NOTOPENED;          
  }   
  
  if (!(WriteFile(HFile, &TestInfo,sizeof(TTestInfo),&Saved, NULL) && (Saved == sizeof(TTestInfo)))){
    return SF_CANTWRITE;                       
  }
  
  for (int i = 0;i < TestInfo.QuestionsC;i++){
    if (!(WriteFile(HFile, &Qs[i]->type, sizeof(int), &Saved, NULL) && (Saved == sizeof(int)))){
      return SF_CANTWRITE;                       
    }
    switch (Qs[i]->type){
      case QT_ABCD:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TABCDQ), &Saved, NULL) && (Saved == sizeof(TABCDQ)))){
          return SF_CANTWRITE;                                            
        }
      } break;
      case QT_BYHAND:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TByHandQ), &Saved, NULL) && (Saved == sizeof(TByHandQ)))){
          return SF_CANTWRITE;                                            
        }   
      } break;
      case QT_GOODORBAD:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TGoodorBadQ), &Saved, NULL) && (Saved == sizeof(TGoodorBadQ)))){
          return SF_CANTWRITE;                                            
        }   
      } break;
      case QT_ESTIMATION:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TEstimationQ), &Saved, NULL) && (Saved == sizeof(TEstimationQ)))){
          return SF_CANTWRITE;                                            
        }   
      } break;       
    }   
  }
  
  CloseHandle(HFile);
  
  return SF_SUCCESS;
  
}

int LoadFromFile(char* FileName,TTestInfo *TestInfo,PQuestion *Qs){
  
  DWORD Loaded;
  HANDLE HFile;
  
  HFile = CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  
  if (HFile == INVALID_HANDLE_VALUE) return LF_NOTOPENED;
  
  if (!(ReadFile(HFile, TestInfo, sizeof(TTestInfo), &Loaded, NULL) && (Loaded == sizeof(TTestInfo)))) return LF_CANTREAD;

  for (int i = 0;i < TestInfo->QuestionsC;i++){
      cout << "Wczytuje pytanie nr " << i+1 << "\n";
    int x;
    if (!(ReadFile(HFile, &x, sizeof(int), &Loaded, NULL) && (Loaded == sizeof(int)))) return LF_CANTREAD;
    switch (x){
      case QT_ABCD:{
        Qs[i] = new TABCDQ;
        if (!(ReadFile(HFile,Qs[i],sizeof(TABCDQ),&Loaded,NULL) && (Loaded == sizeof(TABCDQ)))) return LF_CANTREAD;   
        "Wczytalem TABCDQ\n"; 
        cout << "ilosc odpowiedzi : " << ((TABCDQ*)Qs[i])->AnswersC;
      } break;
      case QT_BYHAND:{
        Qs[i] = new TByHandQ;      
        if (!(ReadFile(HFile,Qs[i],sizeof(TByHandQ),&Loaded,NULL) && (Loaded == sizeof(TByHandQ)))) return LF_CANTREAD;
      } break;
      case QT_GOODORBAD:{
        Qs[i] = new TGoodorBadQ;      
        if (!(ReadFile(HFile,Qs[i],sizeof(TGoodorBadQ),&Loaded,NULL) && (Loaded == sizeof(TGoodorBadQ)))) return LF_CANTREAD;
      } break;
      case QT_ESTIMATION:{
        Qs[i] = new TEstimationQ;      
        if (!(ReadFile(HFile,Qs[i],sizeof(TEstimationQ),&Loaded,NULL) && (Loaded == sizeof(TEstimationQ)))) return LF_CANTREAD;
      } break;       
    }  
  } 
  
  CloseHandle(HFile);
  
  return LF_SUCCESS;
       
}

TTestInfo DefTestInfo(){
          
  TTestInfo res;
  
  res.QuestionsC = 0;
  res.ABCDgp = 80;
  res.ABCDbp = -25;
  res.ABCDtime = 15;
  res.ABCDtimeb = 2;
  res.ABCDft = 1;
  
  res.BHgp = 80;
  res.BHbp = -25;
  res.BHtime = 20;
  res.BHtimeb = 2;
  res.BHft = 2;
  res.BHtip = true;
  res.BHtipstart = 10;
  res.BHtipcost = 1.7;
  
  res.GBgp = 20;
  res.GBbp = -20;
  res.GBtime = 20;
  res.GBtimeb = 2;
  res.GBft = 4;
  
  res.Egp = 80;
  res.Ebp = -25;
  res.Etime = 15;
  res.Etimeb = 2;
  res.Eft = 3;
  res.Epassrange = 0;
  res.Einaccpass = false;
  res.Erange = 0;
  res.Eppay = 0;
  res.Emaxrange = 0;
  
  return res;   
}

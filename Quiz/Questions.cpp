#include "Questions.h"
#include <windows.h>

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

bool SaveToFile(PQuestion *Qs,TTestInfo TestInfo,char* FileName){
  
  DWORD Saved;
  HANDLE HFile;
  
  HFile = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  
  if (HFile == INVALID_HANDLE_VALUE){
    return false;          
  }   
  
  if (!(WriteFile(HFile, &TestInfo,sizeof(TTestInfo),&Saved, NULL) && (Saved == sizeof(TTestInfo)))){
	CloseHandle(HFile);
    return false;                       
  }
  
  for (int i = 0;i < TestInfo.QuestionsC;i++){
    if (!(WriteFile(HFile, &Qs[i]->type, sizeof(int), &Saved, NULL) && (Saved == sizeof(int)))){
	  CloseHandle(HFile);
      return false;                       
    }
    switch (Qs[i]->type){
      case QT_ABCD:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TABCDQ), &Saved, NULL) && (Saved == sizeof(TABCDQ)))){
		  CloseHandle(HFile);
          return false;                                            
        }
      } break;
      case QT_BYHAND:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TByHandQ), &Saved, NULL) && (Saved == sizeof(TByHandQ)))){
		  CloseHandle(HFile);
          return false;                                            
        }   
      } break;
      case QT_GOODORBAD:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TGoodorBadQ), &Saved, NULL) && (Saved == sizeof(TGoodorBadQ)))){
		  CloseHandle(HFile);
          return false;                                            
        }   
      } break;
      case QT_ESTIMATION:{
        if (!(WriteFile(HFile,Qs[i],sizeof(TEstimationQ), &Saved, NULL) && (Saved == sizeof(TEstimationQ)))){
		  CloseHandle(HFile);
          return false;                                            
        }   
      } break;       
    }   
  }
  
  CloseHandle(HFile);
  
  return true;
  
}

bool LoadFromFile(char* FileName,TTestInfo *TestInfo,PQuestion *Qs){
  
  DWORD Loaded;
  HANDLE HFile;
  
  HFile = CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  
  if (HFile == INVALID_HANDLE_VALUE) return false;
  
  if (!(ReadFile(HFile, TestInfo, sizeof(TTestInfo), &Loaded, NULL) && (Loaded == sizeof(TTestInfo)))){
	CloseHandle(HFile);
	return false;
  }

  for (int i = 0;i < TestInfo->QuestionsC;i++){
    int x;
    if (!(ReadFile(HFile, &x, sizeof(int), &Loaded, NULL) && (Loaded == sizeof(int)))){
	CloseHandle(HFile);
	return false;
  }
    switch (x){
      case QT_ABCD:{
        Qs[i] = new TABCDQ;
        if (!(ReadFile(HFile,Qs[i],sizeof(TABCDQ),&Loaded,NULL) && (Loaded == sizeof(TABCDQ)))){
	      CloseHandle(HFile);
	      return false;
        }
      } break;
      case QT_BYHAND:{
        Qs[i] = new TByHandQ;      
        if (!(ReadFile(HFile,Qs[i],sizeof(TByHandQ),&Loaded,NULL) && (Loaded == sizeof(TByHandQ)))){
	      CloseHandle(HFile);
	      return false;
        }
      } break;
      case QT_GOODORBAD:{
        Qs[i] = new TGoodorBadQ;      
        if (!(ReadFile(HFile,Qs[i],sizeof(TGoodorBadQ),&Loaded,NULL) && (Loaded == sizeof(TGoodorBadQ)))){
	      CloseHandle(HFile);
	      return false;
        }
      } break;
      case QT_ESTIMATION:{
        Qs[i] = new TEstimationQ;      
        if (!(ReadFile(HFile,Qs[i],sizeof(TEstimationQ),&Loaded,NULL) && (Loaded == sizeof(TEstimationQ)))){
	      CloseHandle(HFile);
	      return false;
        }
      } break;       
    }  
  } 
  
  CloseHandle(HFile);
  
  return true;
       
}

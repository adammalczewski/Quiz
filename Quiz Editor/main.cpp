#include <cstdlib>
#include <iostream>
#include <windows.h>
#include "TTestForm.h"
#include <string>
using namespace std;

TTestInfo TestInfo;
PQuestion Qs[MAX_QUESTIONS];

using namespace std;

int main(int argc, char *argv[]){
  string x;
  /*TestInfo = DefTestInfo();
  do{
    cin >> x;
    if (x == "/load"){
      string temp;
      cout << "Write File Name : ";
      cin >> temp;
      int res = LoadFromFile(&temp[0],&TestInfo,&Qs[0]);
      if (res == LF_SUCCESS) cout << "File was successfuly loaded\n";
      else if (res == LF_NOTOPENED) cout << "File couldnt have been opened\n";
      else if (res == LF_CANTREAD) cout << "Cant read from file\n";
    } else if (x == "/show"){
      cout << "Number of questions : " << TestInfo.QuestionsC << "\n";
      for (int i = 0;i < TestInfo.QuestionsC;i++){
        cout << i+1 << ". " << Qs[i]->Q.s << " -- " << ((TByHandQ*)Qs[i])->A.s << "\n";    
      }       
    } else if (x == "/addq"){
      string temp;
      cout << "Write question : ";
      cin.ignore(100,'\n');
      getline(cin,temp);
      Qs[TestInfo.QuestionsC] = new TByHandQ;
      Qs[TestInfo.QuestionsC]->defpointset = true;
      Qs[TestInfo.QuestionsC]->deftimeset = true;
      Qs[TestInfo.QuestionsC]->deftimepset = true;
      strcpy(Qs[TestInfo.QuestionsC]->Q.s,&temp[0]);
      ((TByHandQ*)Qs[TestInfo.QuestionsC])->deftipset = true;
      cout << "Write answer : ";
      getline(cin,temp);
      strcpy(((TByHandQ*)Qs[TestInfo.QuestionsC])->A.s,&temp[0]);
      TestInfo.QuestionsC++;     
    } else if (x == "/save"){
      string temp;
      cout << "Write File Name : ";
      cin >> temp;
      int res = SaveToFile(&Qs[0],TestInfo,&temp[0]);
      if (res == SF_SUCCESS) cout << "File was successfuly saved\n";
      else if (res == SF_NOTOPENED) cout << "File couldnt have been opened\n";
      else if (res == SF_CANTWRITE) cout << "Cant write to file\n";
    } else if (x == "/help"){
      cout << "load - loads questions from file\n";
      cout << "save - saves questions to file\n";
      cout << "addq - adds new question\n";
      cout << "show - shows all questions\n";
      cout << "change - changes question\n";
      cout << "cls - clears the screen\n";    
    } else if (x == "/cls"){
      system("cls");      
    } else if (x == "/change"){
      if (TestInfo.QuestionsC < 1){
        cout << "There are no questions to change\n";
        continue;                        
      }
      int qn;
      for (int i = 0;i < TestInfo.QuestionsC;i++){
        cout << i+1 << ". " << Qs[i]->Q.s << " -- " << ((TByHandQ*)Qs[i])->A.s << "\n";    
      }
      cout << "Which question would you like to change ? : ";
      cin >> qn;
      while ((qn < 1) || (qn > TestInfo.QuestionsC)){
        cout << "wrong question number, write again : ";
        cin >> qn;     
      }       
      cout << "You chose : " << qn << ". " << Qs[qn-1]->Q.s << " -- " << ((TByHandQ*)Qs[qn-1])->A.s << "\n";
      cout << "Write new question : ";
      string temp;
      cin.ignore(100,'\n');
      getline(cin,temp);
      strcpy(Qs[qn-1]->Q.s,&temp[0]);
      cout << "Write new answer : ";
      getline(cin,temp);
      strcpy(((TByHandQ*)Qs[qn-1])->A.s,&temp[0]);
    }
  } while (x != "/exit"); */ // BY_HAND
  TestInfo = DefTestInfo();
  do {
	cin >> x;
	if (x == "help"){
      cout << "load - loads questions from file\n";
      cout << "save - saves questions to file\n";
      cout << "addq - adds new question\n";
      cout << "show - shows all questions\n";
      cout << "change - changes question\n";
      cout << "cls - clears the screen\n";  
	} else if (x == "load"){
	  string temp;
      cout << "Write File Name : ";
      cin >> temp;
      int res = LoadFromFile(&temp[0],&TestInfo,&Qs[0]);
      if (res == LF_SUCCESS) cout << "File was successfuly loaded\n";
      else if (res == LF_NOTOPENED) cout << "File couldnt have been opened\n";
      else if (res == LF_CANTREAD) cout << "Cant read from file\n";	
	} else if (x == "save"){
      string temp;
      cout << "Write File Name : ";
      cin >> temp;
      int res = SaveToFile(&Qs[0],TestInfo,&temp[0]);
      if (res == SF_SUCCESS) cout << "File was successfuly saved\n";
      else if (res == SF_NOTOPENED) cout << "File couldnt have been opened\n";
      else if (res == SF_CANTWRITE) cout << "Cant write to file\n";
	} else if (x == "addq"){
      string temp;
      cout << "Write question : ";
      cin.ignore(100,'\n');
      getline(cin,temp);
      Qs[TestInfo.QuestionsC] = new TABCDQ;
      Qs[TestInfo.QuestionsC]->defpointset = true;
      Qs[TestInfo.QuestionsC]->deftimeset = true;
      Qs[TestInfo.QuestionsC]->deftimepset = true;
      strcpy(Qs[TestInfo.QuestionsC]->Q.s,&temp[0]);
      ((TABCDQ*)Qs[TestInfo.QuestionsC])->AnswersC = 4;
      cout << "Write four answers : \n";
      for (int i = 0;i < 4;i++){
		cout << i+1 << ". ";
      	getline(cin,temp);
      	strcpy(((TABCDQ*)Qs[TestInfo.QuestionsC])->As[i].s,&temp[0]);
	  }
	  int tempint;
	  cout << "Which answer is correct ? ";
	  cin >> tempint;
	  ((TABCDQ*)Qs[TestInfo.QuestionsC])->a = tempint-1;
      TestInfo.QuestionsC++; 		
	} else if (x == "show"){
		
	} else if (x == "change"){
		
	} else if (x == "cls"){
	  system("cls");
	}
  } while (x != "exit");
  return 0;
}

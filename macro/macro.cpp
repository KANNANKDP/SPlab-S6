#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<cstring>

using namespace std;

ifstream f1("source");
ofstream f2("target");
ifstream dt("deftab");
int n;
string label,opcode,operand;
bool expanding;

string argtab[100][10];
struct namtab{
  string name;
  int beg,end;
}namtab[100];
int namtabindex=0;
int deftabindex=0;

int searchnamtab(string opcode){
  for(int i=0;i<namtabindex;i++){
    if (namtab[i].name==opcode) {
      return i;
    }
  }
  return -1;
}

void getline(){
    if(expanding){

      dt>>label>>opcode>>operand;
      for(int i=0;i<10;i++){
        char c=i+'0';
        string c1(1,c);
        string op="postnot"+c1;
        if(operand==op){
          cout<<"argtab:"<<argtab[n][0]<<argtab[n][1];
          cout<<endl<<i;
          operand=argtab[n][i];
          cout<<operand<<endl;
          break;
        }
      }
    }
    else{
      f1>>label>>opcode>>operand;
    }
}


void define(){
  namtab[namtabindex].name=label;
  namtab[namtabindex].beg=deftabindex;
  ofstream dt1("deftab");
  dt1.seekp(deftabindex);
  dt1<<endl<<label<<"    "<<operand;
  int level=1;
  string args[10];int argindex=0;
  int i;int j=0;
  for(i=0;i<operand.length();i++){
    if(operand[i]==','){
      args[argindex++]=operand.substr(j,i);j=i+1;}
  }
  args[argindex++]=operand.substr(j);



  while (level>0) {
    getline();
    bool flag=true;

    dt1<<endl<<label<<"   ";
    if(opcode=="macro")
      level=level+1;
    else if (opcode=="mend") {
      level=level-1;
    }
    dt1<<opcode<<"   ";

    for(int i=0;i<argindex;i++)
      if (args[i]==operand) {
        dt1<<"postnot"<<i;flag=false;break;
      }

    if (flag) {
      dt1<<operand;
    }
  }
  deftabindex=dt1.tellp();

  namtab[namtabindex++].end=dt1.tellp();
  dt1.close();
}


void expand(int);

void processline(){

  if (searchnamtab(opcode)!=-1) {
    expand(searchnamtab(opcode));
  }
  else if (opcode=="macro") {
    define();
  }
  else{
    f2<<setw(8)<<label<<"   "<<opcode<<setw(13)<<operand<<endl;
  }
}



void expand(int n){
  expanding=true;
  dt.seekg(namtab[n].beg);
  int i;int j=0;int argtabindex=0;
  for(i=0;i<operand.length();i++){
    if (operand[i]==',') {
      argtab[n][argtabindex++]=operand.substr(j,i);
      j=i;
    }
  }
      argtab[n][argtabindex]=operand.substr(j+1,i);
  string l,c,o;
  dt>>l>>o;
    n=searchnamtab(opcode);
  do{
    getline();
    processline();
  }while(opcode!="mend");
  expanding=false;
}

int main(){
  expanding = false;

  do{
    getline();
    processline();
  }while(opcode!="end");
}

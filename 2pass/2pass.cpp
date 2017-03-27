//Abhishek Suresh
//Program to implement a 2 pass assembler.
//20/3/2017
#include<iostream>
#include<string>
#include<fstream>

using namespace std;

string opcode;
string label;
char operand[10];
int startaddr;
int locctr;

struct optab{
string instruction;
int opcode;
}optab[100];

struct symtab{
string symbol;
int symcode;
}symtab[100];

int optabsize=0,symtabsize=0,programlength=0;

void optabgen(){
        ifstream f1;
        int i;
        f1.open("optab");
        for( i=0;i<100||f1;i++){
                f1>>optab[i].instruction;
                f1>>dec>>optab[i].opcode;
        }
        optabsize=i;
        f1.close();
}
void symtabgen(string label,int locctr){

        symtab[symtabsize].symbol=label;
        symtab[symtabsize].symcode=locctr;
        symtabsize++;
}

int searchsymtab(string key){
        for(int i=0;i<symtabsize;i++){
                if(key==symtab[i].symbol)
                        return symtab[i].symcode;
        }
        return -1;
}
int searchoptab(string key){
        for(int i=0;i<optabsize;i++){
                if(optab[i].instruction==(key))
                        return optab[i].opcode;
        }
        return -1;
}
void initTRecord(ifstream &f1,ofstream &f2){

}

int pass1(){
        ifstream f1;
        ofstream f2;
        f1.open("source5");
        f2.open("intermediate");
        optabgen();
        f1>>label;
        f1>>opcode;
        if (opcode=="START"){
                f1>>hex>>startaddr;
                locctr=startaddr;
                f2<<hex<<startaddr<<" "<<label<<" "<<opcode<<" "<<hex<<startaddr<<endl;

        }
        else{
                locctr=0;
        }
        f1>>label;
        f1>>opcode;
        f1>>operand;

        while(opcode !="END"){
                f2<<hex<<locctr<<"  "<<label<<"  "<<opcode<<"  "<<operand<<endl;
                if(label!="0"){
                        if(searchsymtab(label)!=-1){
                                cout<<"Error duplicate label";
                                return 0;
                        }
                        else {
                                symtabgen(label,locctr);
                        }
                }
                if(searchoptab(opcode)!=-1)
                        locctr+=3;
                else if(opcode==("WORD"))
                        locctr+=3;
                else if(opcode==("RESW"))
                        locctr+=3*atoi(operand);
                else if(opcode==("RESB"))
                        locctr+=atoi(operand);
                else if(opcode==("BYTE"))
                        {}// fill in later
                else {
                        cout<<"ERROR OPCODE NOT FOUND";
                        return 0;
                }
                f1>>label;
                f1>>opcode;
                f1>>operand;
                }
         f2<<hex<<locctr<<"  "<<label<<"  "<<opcode<<"  "<<operand<<endl;
         programlength=locctr;
         f1.close();
         f2.close();
 f1.open("intermediate");
         f2.open("target");
        }


/*input




*/

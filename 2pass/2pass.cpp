//Abhishek Suresh
//Program to implement a 2 pass assembler.
//20/3/2017
#include<iostream>
#include<string>
#include<fstream>
#include<cstdlib>
#include<iomanip>
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

int pass1(){
        ifstream f1;
        ofstream f2;
        f1.open("source5");
        f2.open("intermediate");
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
         programlength=locctr-startaddr;
         f1.close();
         f2.close();
        }

int p;
void initTRecord(ifstream &f1,ofstream &f2){
	f2<<"T "<<setw(6)<<setfill('0')<<hex<<locctr<<" "	;
	p=f2.tellp();
	f2<<"00 ";	
}
int pass2(){
	ifstream f1;
	ofstream f2;	
	f1.open("intermediate");	
        f2.open("target");
	f1>>hex>>locctr>>label>>opcode>>hex>>startaddr;
	f2<<"H "<<label<<" "<<hex<<startaddr<<" "<<hex<<programlength<<endl;
	f1>>hex>>locctr>>label>>opcode>>operand;
 	initTRecord(f1,f2);
	while(opcode!="END"){
		if(searchoptab(opcode)!=-1){
			f2<<setw(2)<<setfill('0')<<searchoptab(opcode);
			if(operand!=""){
				if(searchsymtab(operand)!=-1){
					f2<<setw(4)<<setfill('0')<<searchsymtab(operand)<<" ";
					}
				else{
					cout<<"undefined symbol";
						return -1;
					}
			}
		}
		f1>>hex>>locctr>>label>>opcode>>operand;
	}
					

}
int main(){
	optabgen();
	if(pass1()==-1)
		return 0;
	else if(pass2()==-1)
		return 0;
	else
		return 1;
				
	}



//Program to implement a 1 pass assembler.
//Abhishek Suresh
//23/3/2017
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<string.h>
#include<stdlib.h>
using namespace std;

string opcode;
string label;
char operand[10];
int startaddr;
int locctr;
ifstream f1;
ofstream f2;

struct optab{
	string mnemonic;
	int opcode;
	}optab[100];

struct symtab{
	string label;
	int addr;
	bool defined;
	struct node *fwdref;
	}symtab[100];

struct node{
	int ref;
	struct node* link;
	};

int symtablength = 0 , optablength = 0 , programlenth = 0 ;

void generateoptab(){
	ifstream f;
	int i=0;
	f.open("optab");
	for (i=0;f1&&i<100;i++){
		f>>optab[i].mnemonic;
		f>>hex>>optab[i].opcode;
		}
	optablength=i;
	f.close();
	}
int searchoptab(string key){
	for(int i=0;i<optablength;i++){
		if(optab[i].mnemonic==key){
			return i;
			}
		}
	return -1;
	}

int searchsymtab(string key){
	for(int i=0;i<symtablength;i++){
		if(symtab[i].label==key){
			return i;
			}
		}
	return -1;
	}

int p;int TRlen=0;

void initTRecord(){
		f2<<endl<<"T "<<setw(6)<<setfill('0')<<hex<<locctr<<" "	;
		p=f2.tellp();
		f2<<"00 ";
		TRlen=0;
	}

void insertsymtab(int labelsearchresult,char encounter){
	if(labelsearchresult==-1){//first encounter of symbol.
		if(encounter=='l'){//encountered in label field
			symtab[symtablength].label=label;
			symtab[symtablength].addr=locctr;
			symtab[symtablength].defined==true;
		}
		else if(encounter=='o'){//encountered in operand field
			symtab[symtablength].label=label;
			symtab[symtablength].defined=false;
			symtab[symtablength].fwdref->ref=f2.tellp();
			symtab[symtablength].fwdref->link=NULL;
		}
		symtablength++;
	}
	else{
		if(encounter=='o'&&symtab[labelsearchresult].defined==false){
			node *ptr=symtab[symtablength].fwdref;
				while(ptr->link!=NULL)
					ptr=ptr->link;
			ptr->link=new node;
			ptr=ptr->link;
			ptr->ref=f2.tellp();
			ptr->link=NULL;
		}
		else if(encounter =='l' && symtab[labelsearchresult].defined==false){
			symtab[labelsearchresult].addr=locctr;
			symtab[labelsearchresult].defined==true;
		}
		else if(encounter =='l' && symtab[labelsearchresult].defined==true)
			cout<<"Error duplicate label";
	}

}

int constlen(string key){
	int pos=0;
	if (key[pos++]=='X'){
		if(key[pos]=='\''){
			pos++;
			while(key[pos]!='\''){
				pos++;
			}
			pos-=3;
		}
		else{
			cout<<"Error invalid constant assigned to BYTE";
			return -1;
		}
		return pos;
	}
	//else for future elaboration.
	return key.length();
}

int main(){
	int programlengthpos;
	f1.open("source5");
	f2.open("target",ios::trunc);
	generateoptab();
	f1>>label>>opcode>>hex>>startaddr;
	if (opcode=="START"){
		f2<<"H "<<label<<" "<<setw(6)<<setfill('0')<<hex<<startaddr<<" ";
		locctr=startaddr;
		programlengthpos=f2.tellp();
		f2<<setw(6)<<setfill('0');
	}
	else
		locctr=0;
	f1>>label>>opcode>>operand;
 	initTRecord();
	while(opcode!="END"){
		if(label!="0"){//label in symbol field
						insertsymtab(searchsymtab(label),'l');//0 means encounter in label.
						}

		if(searchoptab(opcode)!=-1){
			f2<<setw(2)<<setfill('0')<<searchoptab(opcode);TRlen+=3;
			if(operand!=""){
				if(searchsymtab(operand)!=-1&&symtab[searchsymtab(operand)].defined==true){
					f2<<setw(4)<<setfill('0')<<searchsymtab(operand)<<" ";
					}
				else{
					insertsymtab(searchsymtab(label),'o');
					}
			}
			locctr+=3;
		}
		else if(opcode==("WORD"))
						locctr+=3;
		else if(opcode==("RESW"))
						locctr+=3*atoi(operand);
		else if(opcode==("RESB"))
						locctr+=atoi(operand);
		else if(opcode==("BYTE"))
						{if(constlen(operand)!=-1)
							locctr+=constlen(operand);
							else
							return 0;
						}
		else {
			cout<<"ERROR OPCODE NOT FOUND"<<opcode;
			return 0;
		}
cout<<locctr;

	}
}

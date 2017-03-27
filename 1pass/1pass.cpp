//Program to implement a 1 pass assembler.
//Abhishek Suresh
//23/3/2017
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

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
	ifstream f1;
	int i=0;
	f1.open("optab");
	for (i=0;f1;i++){
		f1>>optab[i].mnemonic;
		f1>>hex>>optab[i].opcode;
		}
	optablength=i;
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
			return i
			}
		}
	return -1;
	}

int main(){};

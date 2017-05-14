//Abhishek Suresh
//Program to implement a assembler that also generates modification records.
//12/4/2017
#include<iostream>
#include<string>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<iomanip>
//Define maximum length of text record.
#define maxlen 18;

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
//function to generate optab array from optab file.
void optabgen(){
        ifstream f1;
        int i;
        f1.open("optab");
        for( i=0;i<100&&f1;i++){
                f1>>optab[i].instruction;
                f1>>hex>>optab[i].opcode;
        }
        optabsize=i;
        f1.close();
}
//fucntion to add each label to symtab.
void symtabgen(string label,int locctr){

        symtab[symtabsize].symbol=label;
        symtab[symtabsize].symcode=locctr;
        symtabsize++;
}
//function to search symtab for a particular label.
int searchsymtab(string key){
        for(int i=0;i<symtabsize;i++){
                if(key==symtab[i].symbol)
                        return symtab[i].symcode;
        }
        return -1;
}
//function to search optab for a particular opcode.
int searchoptab(string key){
        for(int i=0;i<optabsize;i++){
                if(optab[i].instruction==(key))
                        return optab[i].opcode;
        }
        return -1;
}

//function to return length of constant
int constlen(string key){
	int pos=0;
	if (key[pos++]=='X'){
		if(key[pos]=='\''){
			pos++;
			while(key[pos]!='\''){
				pos++;
			}
			pos-=2;//returning length of constant within quotes.
		}
		return pos;
	}
	//else for future elaboration.
	return key.length();
}

int relocpos[100];

int relocnum=0;

void initMR(ifstream &f1, ofstream &f2){//Function to create Modification Record.
	for(int i=0;i<relocnum;i++){
		f2<<endl<<"M "<<setw(6)<<setfill('0')<<hex<<relocpos[i]<<" "<<"05";
		}
	}

//pass 1 fills symbol table and finds length of program.
int pass1(){
        ifstream f1;
        ofstream f2;
        f1.open("source7");
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
                if(label!="0"){//if there is a label.
			//if searchsymtab doesnot find label it returns -1.
                        if(searchsymtab(label)!=-1){
                                cout<<"Error duplicate label";
                                return 0;
                        }
                        else {//adding label to symtab.
                                symtabgen(label,locctr);
                        }
                }
		//returns -1 if opcode is not valid.
                if(searchoptab(opcode)!=-1){
			if(opcode[0]!='+')
                        	locctr+=3;//each instruction takes 3
			else
				locctr+=4;//if extended addressing format
		//one word needs 3
		}
                else if(opcode==("WORD"))
                        locctr+=3;
		//each word takes 3
                else if(opcode==("RESW"))
                        locctr+=3*atoi(operand);
		//get number of bytes
                else if(opcode==("RESB"))
                        locctr+=atoi(operand);
                else if(opcode==("BYTE"))//get length of constant in bytes.
                        {if(constlen(operand)!=-1)
				locctr+=(constlen(operand)/2);
			 else
				return 0;
			}
                else {
                        cout<<"ERROR OPCODE NOT FOUND";
                        return 0;
                }
                f1>>label;//read next line.
                f1>>opcode;
                f1>>operand;
         }
		//write last line
         f2<<hex<<locctr<<"  "<<label<<"  "<<opcode<<"  "<<operand<<endl;
         programlength=locctr-startaddr;//store program length
         f1.close();
         f2.close();
}

int p;//holds position of textrecord where lenth has to be filled in later.
int TRlen =0;//holds current length of text record.

//function to initialise a text record.
void initTRecord(ifstream &f1,ofstream &f2){
	f2<<endl<<"T "<<setw(6)<<setfill('0')<<hex<<locctr<<" "	;
	p=f2.tellp();//saving location to write length.
	f2<<"00 ";//placeholder value for text record length	
}


int pass2(){
	ifstream f1;
	ofstream f2;	
	f1.open("intermediate");	
        f2.open("target");
	//read first line
	f1>>hex>>locctr>>label>>opcode>>hex>>startaddr;
	//create header record
	f2<<"H "<<label<<" "<<setw(6)<<setfill('0')<<hex<<startaddr<<" "<<setw(6)<<setfill('0')<<hex<<programlength;
	//read next line
	f1>>hex>>locctr>>label>>opcode>>operand;
	//initialise first text record.
 	initTRecord(f1,f2);
	while(opcode!="END"){
		if(searchoptab(opcode)!=-1){//if opcode found
			//place corresponding opcode value in text record.
			if(opcode[0]!='+'){
				f2<<setw(2)<<setfill('0')<<searchoptab(opcode);TRlen+=3;
			}
			else {  //relocation is required
				relocpos[relocnum]=TRlen+1;
				relocnum++;
				f2<<setw(3)<<setfill('0')<<searchoptab(opcode);TRlen+=4;
			}
			if(operand!=""){//if operand is present
				if(searchsymtab(operand)!=-1){//search symtab
					//if found place address in text record.
					if(opcode[0]!='+')
					f2<<setw(4)<<setfill('0')<<searchsymtab(operand)<<" ";
					else//relocation is required.
					f2<<setw(5)<<setfill('0')<<searchsymtab(operand)<<" ";
					}
				else{//symbol not in symtab
					cout<<"undefined symbol";
						return -1;
					}
			}
		}
		//convert constant to object code.
		else if(opcode=="WORD"){
			f2<<setw(6)<<setfill('0')<<dec<<atoi(operand)<<" ";
			TRlen+=3;
		}
		else if(opcode=="BYTE"){
			if(operand[0]='X'){
				f2<<operand[2]<<operand[3]<<" ";
				TRlen+=2;
			}
			else{
				f2<<operand<<" ";
				TRlen+=strlen(operand);
			}
	
		}
		//if res is encountered start a new text record.
		else if(opcode=="RESW"||opcode=="RESB"){
			int c=f2.tellp();//save current position.
			f2.seekp(p);//go to position in text record to write length.
			f2<<setw(2)<<setfill('0')<<hex<<TRlen;//write text record length.
			f2.seekp(c);//go back to end of file.
			TRlen=0;//reset text record lenth.
			while(opcode=="RESW"||opcode=="RESB"){
				//if many res comes succesively don't create new text record.
				f1>>hex>>locctr>>label>>opcode>>operand;
			}//don't create new record if end reached.
			if(opcode!="END")
				initTRecord(f1,f2);
			continue;
		}
					
		f1>>hex>>locctr>>label>>opcode>>operand;//read next record;

		if(TRlen>=15||opcode=="END"){//if text record reached maximum length.
			int c=f2.tellp();
			f2.seekp(p);
			f2<<setw(2)<<setfill('0')<<hex<<TRlen;
			f2.seekp(c);
			TRlen=0;
			if(opcode!="END"){
				initTRecord(f1,f2);
			}
		}
	}
	initMR(f1,f2);
	//write end record.
	f2<<endl<<"E "<<setw(6)<<setfill('0')<<hex<<startaddr;
					

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

/*----------------------END OF PROGRAM.----------------------------------------------------------------
INPUT

RDREC  START 1000
0      LDX   ZERO
0      LDA   ZERO
RLOOP  TD    INPUT
0      JEQ   RLOOP
0      RD    INPUT
0      COMP  ZERO
0      JEQ   EXIT
0      STCH  BUFFER
0      TIX   MAXLEN
0      +JLT   RLOOP
EXIT   STX   LENGTH
INPUT  BYTE  X'F1'
MAXLEN WORD  4096
BUFFER RESB  4096
ZERO   WORD  0
LENGTH RESW  1
0      END   RDREC

OPTAB

LDX  1
LDA  2
TD   3
JEQ  4
RD   5
COMP 6
STCH 7
TIX  8
JLT  9
STX  10
+JLT 4b1

OUTPUT

H RDREC 001000 00102c
T 001000 0f 012026 022026 031022 041006 051022
T 00100f 10 062026 04101f 071026 081023 4b101006
T 00101f 08 102029 F1 004096
T 002026 03 000000
M 00000d 05
E 001000

---------------------------------------------------------------------------------------------*/

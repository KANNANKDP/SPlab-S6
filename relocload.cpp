//Abhishek Suresh
//Program to implement a relocating loader with modification record.
//30-1-2017
#include<iostream>
#include<fstream>

using namespace std;


main(){ 
        char ch;int locctr;int flag=0;char s[100];int l;int n1,n2,n3;int i;
	int locctr2,locctr3;
	int base;int hb;int startaddr;
        ifstream f1;
        ofstream f2;
	ofstream f3;
	ifstream f4;
        cout<<"input starting address";
        cin>>hex>>startaddr;
        f1.open("source3");
        f2.open("intermediate",ios::trunc);
	f3.open("target",ios::trunc);
        f1>>s;
        while(s[0]!='M')//creating intermediate file without relocation.
                f1>>s;
                while(s[0]=='T'){
                        f1>>hex>>locctr;
			locctr+=startaddr;//adding to relocate.
                        f1>>hex>>l;
                        f1>>s;
			for(int i=0;i<l*2;i+=2){
                                f2<<hex<<locctr<<"   ";
                                f2<<s[i]<<s[i+1]<<"\n";
                                locctr=locctr+1;
                        }        
			  f1>>s;

                }

        }
	f2.close();
	f4.open("intermediate");
	while(s[0]!='E'){//reading each modification record.
		f1>>hex>>base;	
		base+=startaddr;
		f1>>hex>>hb;
		f4>>hex>>locctr;
		while(locctr!=base){
			f3<<hex<<locctr;
			f3<<"  ";
			f4>>s;
			f3<<s<<endl;
			f4>>hex>>locctr;
		}
		f4>>hex>>n1;
		//creating address to add startaddress
		n1=n1*0x100;
		f4>>hex>>locctr2;
		f4>>hex>>n2;
               
		n1+=n2;
		n1*=0x100;
		f4>>hex>>locctr3;
		f4>>hex>>n3;
                
		n1+=n3;

		n1+=startaddr;//adding startaddress

		int copy=n1;
		//splitting address again to load.
		n1=copy&0xff0000;
		n1=n1>>16;
		n2=copy&0x00ff00;
		n2=n2>>8;
		n3=copy&0x0000ff;

		f3<<hex<<locctr;
		f3<<"  ";
		f3<<hex<<n1;
		f3<<endl;

		f3<<hex<<locctr2;
		f3<<"  ";
		f3<<hex<<n2;
		f3<<endl;

		f3<<hex<<locctr3;
   		f3<<"  ";
		f3<<hex<<n3;
		f3<<endl;

		f1>>s;
	}

}				

//---------------------------END OF PROGRAM----------------------------------------



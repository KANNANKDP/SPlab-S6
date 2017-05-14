//Abhishek Suresh
//Program to implement a absolute loader.
//30-1-2017
#include<iostream>
#include<fstream>

using namespace std;

main(){ 
        char ch;int locctr;int flag=0;char s[100];int l;int n;int i;
        ifstream f1;
        ofstream f2;
        ofstream f3;
        ofstream f4;
        f1.open("source2");
        f2.open("target",ios::trunc);
        f1>>s;
        while(s[0]!='E'){//checking for end record.
                f1>>s;
                while(s[0]=='T'){//checking each text record.
                        f1>>hex>>locctr;
                        f1>>hex>>l;
                        f1>>s;
                        for(int i=0;i<l*2;i+=2){//dividing each line of text record.
                                f2<<hex<<locctr<<"   ";
                                f2<<s[i]<<s[i+1]<<"\n";
                                locctr=locctr+1;
                        }
                          f1>>s;

                }

        }
}
//----------------------------END OF PROGRAM---------------------------------------------------

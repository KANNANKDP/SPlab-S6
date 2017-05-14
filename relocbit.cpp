//Abhishek Suresh
//Program to implement a relocating loader with modification bit.
//30-1-2017
#include<iostream>
#include<fstream>

using namespace std;

main(){
        char ch;int locctr;int s;char sa[100];int l;int n1,n2,n3,n4;int i;
        int startaddr;char p[2];int p1,p2;int ba[100],b[100];int j;int copy;
        ifstream f1;
        ofstream f2;
        cout<<"input starting address";
        cin>>hex>>startaddr;
        f1.open("source4");
        f2.open("target",ios::trunc);
        f1>>sa;f1>>sa;
                while(sa[0]=='T'){
                        f1>>hex>>locctr;
                        locctr+=startaddr;
                        f1>>hex>>l;
			f1>>p;
			i=0;
			//obtaining relocation bit
			p1=p[0]-48;
			p2=p[1]-48;
			//converting to binary
			while(p1>0){
				b[i]=p1%2;
				p1/=2;i++;
			}
			//adding remaining 0s
			for(;i<4;i++)
				b[i]=0;
			//converting to binary
			while(p2>0){
                                b[i]=p2%2;
                                p2/=2;i++;
                        }
			//adding remaining 0s
			for(;i<8;i++)
				b[i]=0;
			// reversing the binary array
			for(i=3,j=0;i>=0;i--,j++)
				ba[j]=b[i];
			for(i=7,j=4;i>=4;i--,j++)
				ba[j]=b[i];
			
			j=0;
			//extracting each operation
                        for(i=0;i<l/3;i++){
                                f2<<hex<<locctr++<<"   ";
				f1>>hex>>s;
		
				copy=s;
				if(ba[j]==1){//relocation required
					copy+=startaddr;
					n1=copy&0xff000000;
					n1=n1>>24;
					f2<<hex<<n1<<endl<<hex<<locctr++<<"   ";
				}
				j++;
				n2=copy&0x00ff0000;
                                n2=n2>>16;
                                n3=copy&0x0000ff00;
                                n3=n3>>8;

                                n4=copy&0x000000ff;
				f2<<hex<<n2<<endl;
				f2<<hex<<locctr++<<"   ";
                                f2<<hex<<n3<<endl;
				f2<<hex<<locctr++<<"   ";
                                f2<<hex<<n4<<endl;
				
                        }
			  
                          f1>>sa;
        }
}

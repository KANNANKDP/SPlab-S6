//Abhishek Suresh
//Program to implement a recursive descent parser.
//3-5-2017

#include<iostream>
#include<string>
#include<fstream>
using namespace std;

string exp;
int i;
int E();
int F(){
	if(exp[i]=='('){
		i++;
		if(E())
			if(exp[i]==')'){
				i++;
				return 1;
			}
	}
	else if(exp[i]>=97&&exp[i]<=122){
		i++;
		return 1;
	}
	return 0;
}

int TPRIME(){
	if(exp[i]=='*'){
		i++;
		if(F())
			if(TPRIME())
				return 1;
	return 0;
	}
	return 1;
	
}

int T(){
	if(F())
		if(TPRIME())
			return 1;
return 0;
}

int EPRIME(){
	if(exp[i]=='+'){
		i++;
		if (T())
			if(EPRIME())
				return 1;
	return 0;
	}
	return 1;
}

int E(){
	if (T()){
		if (EPRIME()){
			return 1;
		}
		return 0;
	}
	return 0;
}

int main(){
	cout<<"Enter the expression to be parsed";
	cin>>exp;
	if (E()==1){
		if(i==exp.length())
		cout<<"accept";
		else
		cout<<"reject";
	}
	else
		cout<<"reject";
}

//-----------------------------------END OF PROGRAM-----------------------------------

// Abhishek_Suresh
// Program_to_implement_lexical_analyzer.
// 17/4/2017

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<ctype.h>
using namespace std ;
string keywords [ 17 ] = { "int" , "char" , "double" , "return" , "using" , "namespace" , "if" , "do" , "while" , "for" , "endl" , "ifstream" , "ofstream" } ;
string relop [ 5 ] = { "<" , ">" , "==" , "<=" , ">=" } ;
string op[ 7 ] = { "+" , "-" , "*" , "/" , "." , "=" , "<<" } ;
string spec[ 9 ] = { "," , ";" , "(" , ")" , "[" , "]" , "{" , "}" , "." } ; 
string lexeme ;

ifstream f1 ( "lex.cpp" ) ;
ofstream f2 ( "target" ) ;
int check ( string lexeme ) { 
	string le = lexeme . substr ( 0 , 1 ) ;
        if ( le == "#" ) { 
      		 f2 << "< " << lexeme << " , preproccesor directive >" << endl ;
		 return 0 ;
	}
	if (lexeme == "//") 
	{
		f1 >> lexeme ;
		return 0 ;
	}
	if ( le == "'" ) 
	{
		 int l = lexeme . length ( ) ;
                 if ( le == "'" ) 
		 {
                        f2 << lexeme . substr ( 1 , l - 2 ) << " , literal" << endl ;
                        return 0 ;
                 } 
		 return 0 ;
	}
	if ( lexeme [ 0 ] == '"' ) {
		 int l = lexeme . length ( ) ;
		 if ( lexeme [l-1] == '"' ) {
			f2 << lexeme . substr ( 1 , l - 2 ) << ",literal" << endl ;
			return 0 ;
			}
		 return 0 ;
	}
	for ( int i = 0 ; i < 5 ; i = i + 1  ) {
		if ( relop [ i ] == lexeme ) {
			f2 << lexeme << ",relational operator" << endl ;
			return 0 ;
		}
	}
	for ( int i = 0 ; i < 7 ; i = i + 1 ) {
		if ( op [ i ] == lexeme ) {
			f2 << lexeme << ",operator" << endl ;
			return 0 ;
		}
	}
	for ( int i = 0 ; i < 17 ; i = i + 1 ) {
		if ( keywords [ i ] == lexeme ) {
			f2 << lexeme << ",keyword" << endl ;
			return 0 ;
			}
	}
	for ( int i = 0 ; i < 9 ; i = i + 1 ) {
		if ( spec [ i ] == lexeme ) {
			f2 << lexeme << ",special" << endl ;
			return 0 ;
			}
	}
	if ( isdigit ( lexeme [ 0 ]  ) ) {
			f2 << lexeme << ",constant" << endl ;
			return 0 ;
	}

	f2 << lexeme << ",identifier" << endl ;
	return 0 ;
}

int main ( ) {
	while ( f1 ) {
		f1 >> lexeme;
	 	int i = check ( lexeme ) ;
		if ( i == 1 ) { 
			cout << "ERROR" ; 
			return 0 ;
		}
	}
}
 		
// END OF PROGRAM
 



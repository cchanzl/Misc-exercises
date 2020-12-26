#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

string aa = "0....."; // 1
string bb = "00...."; // 2
string cc = "0..0.."; // 3
string dd = "0..00."; // 4
string ee = "0...0."; // 5
string ff = "00.0.."; // 6
string gg = "00.00."; // 7
string hh = "00..0."; // 8
string ii = ".0.0.."; // 9
string jj = ".0.00."; // 0
string kk = "0.0...";
string ll = "000...";
string mm = "0.00..";
string nn = "0.000.";
string oo = "0.0.0.";
string pp = "0000..";
string qq = "00000.";
string rr = "000.0.";
string ss = ".000..";
string tt = ".0000.";
string uu = "0.0..0";
string vv = "000..0";
string ww = ".0.000";
string xx = "0.00.0";
string yy = "0.0000";
string zz = "0.0.00";

string alphabets[26] = {aa,bb,cc,dd,ee,ff,gg,hh,ii,jj,kk,ll,mm,nn,oo,pp,qq,rr,ss,tt,uu,vv,ww,xx,yy,zz};

char aSymbol[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm' ,'n', 'o', 'p', 'q' , 'r', 's' ,'t', 'u', 'v', 'w', 'x', 'y', 'z'};
char pSymbol[8] = {'.', ',', ';', '-', '!', '?', '(', ')'};

  
void encode_numeric(const char ch, char braille[]) {
  int index(0);
  braille[index++] = '.';
  braille[index++] = '.';
  for ( ; index < 6; index++) braille[index] = '0';
  
  int pos = static_cast<int>(ch) - 48;

  if ( ch == '0' ) strcpy(braille+index, alphabets[10].c_str());
  else strcpy(braille+index, alphabets[pos - 1].c_str());
}

void encode_alpha(char ch, char braille[]){
  int index(0);
  
  if ( isupper(ch) ){
    for ( int i = 0; i < 5; i++) braille[i] = '.';
    braille[5] = '0';
    index = 6;
  }

  ch = toupper(ch);
  int pos = static_cast<int>(ch) - 65;
  
  strcpy(braille+index, alphabets[pos].c_str());
  
}

void encode_punct(char ch, char braille[]){
  string period = ".0..00";
  string comma = ".0....";
  string semi = ".00...";
  string dash = "..0..0";
  string excl = ".00.0.";
  string qn = ".00..0";
  string bracket = ".00.00";

  int count(8);
  string punctuation[count] = {period, comma, semi, dash, excl, qn, bracket, bracket};

  for ( int i = 0; i < 8; i++){
    if ( pSymbol[i] == ch ) strcpy(braille, punctuation[i].c_str());
  }
}


int encode_character(char ch, char braille[], int start){

  int count(0);
  int dots = 6;

  // clean up braille
  if ( start == 0 ){
    for ( int i = 0; i < 512; i++) braille[i] = 0;
  }
  
  if ( isalpha(ch) ) {
    encode_alpha(ch, braille+start);
    count = dots + (dots * (isupper(ch)? 1:0) );
  }
  else if ( isdigit(ch) ) {
    encode_numeric(ch, braille+start);
    count = 2*dots;
  }
  else if ( ispunct(ch) ) {
    encode_punct(ch, braille+start);
    count = dots;
  }
    
  return count;
}

string copy_str(const string original, const int end){
  string copied = "";
  int start(1);
  
  while(original[start] != '\0'){
    copied = copied + original[start];
    start++;
  }

  return copied;
  
}

void encode(string plaintext, char braille[], int count){
  char ch = plaintext[0];
  int length = plaintext.length();
  string plaintext2 = copy_str(plaintext, length);

  if ( length == 0 ) return;
  
  if ( isalpha(ch) || isdigit(ch) || ispunct(ch) ) {
    encode_character(ch, braille, count);
    //cout << braille << endl;
    if ( isupper(ch) || isdigit(ch) ) count += 12;
    else count += 6;
  }

  encode(plaintext2, braille, count);    

  return;
  
}


void print_braille(string plaintext, ostream& o){
  char braille[512];

  // encode the array of braille first
  encode(plaintext, braille, 0);

  // count number of non-empty array
  int count(0);
  while ( braille[count] == '.' || braille[count] == '0' ){
    count++;
  }

  int offset = 0;
  for ( int r = 0; r < 4; r++){

    if ( r == 3 ){
      int index(0);
      while ( plaintext[index] != '\0' ){
	char ch = plaintext[index];
	
	if ( isalpha(ch) ) {
	  for ( int i = 0; i < 26; i++){
	    if ( aSymbol[i] != tolower(ch) ) continue;
	    bool case_gate = false;
	    if (  isupper(ch) ) {
	      ch = tolower(ch);
	      case_gate = true;
	    }
	    
	    (case_gate)? o << "   " <<  static_cast<char>(toupper(ch)) : o << ch;
	    o << "  ";
	    break;
	  }
	}

	else if ( ispunct(ch) ) {
	  for ( int i = 0; i < 8; i++) if ( pSymbol[i] == ch ) o << ch << "  ";
	}

	else if ( isdigit(ch) ) o << "   "  << static_cast<int>(ch)-48 << "  ";
	  
	index++;
      }
    }
    
    else {
      for ( int c = 0+offset; c < count; c += 6) o << braille[c] << braille[c+3] << " ";
    }
    
    cout << endl;
    offset +=1;
  }
  
}


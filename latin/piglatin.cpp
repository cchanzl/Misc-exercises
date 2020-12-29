#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

vector<char> vowel = {'a', 'e', 'i', 'o', 'u'};

bool is_letter(char letter, vector<char> vowel){
  int size = vowel.size();
  for ( int i = 0; i < size; i++){
    if ( letter == vowel[i] ) return true;
  }
  return false;
}

int findFirstVowel(string word){
  int size = word.length();
  const char* letter = word.c_str();

  int count (0);
  while ( *letter != '\0') {

    char alphabet = tolower(*letter);
    if ( alphabet == 'y' && count >0 && count < size-1 ) return count;
    if ( is_letter(alphabet, vowel) ) return count;
					      
    letter++;
    count++;
  }

  return -1;

}

void translateWord ( string word, char translated[]){

  // clear translated
  for ( int i = 0; i < 100; i++) translated[i] = '\0';
  
  int size = word.length();
  const char* letter = word.c_str();

  // word begins with a character
  if ( isdigit(*letter) ){
     for ( int i = 0; i < size; i++){
      translated[i] = *(letter + i);
    }
     return;
  }
  
  // word begins with a vowel
  if ( findFirstVowel(word) == 0 ){
    for ( int i = 0; i < size; i++){
      translated[i] = *(letter + i);
    }
    translated[size++] = 'w';
    translated[size++] = 'a';
    translated[size++] = 'y';
    return;
  }

  // no vowels
  if ( findFirstVowel(word) == -1 ){
     for ( int i = 0; i < size; i++){
      translated[i] = *(letter + i);
    }
    translated[size++] = 'a';
    translated[size++] = 'y';
    return;
  }
  
  // word does not start with vowel
  bool case_gate = false;
  if ( isupper(*letter) ) case_gate = true;
  int start = findFirstVowel(word);
  int index(0);
  for ( int i = 0; i < size; i++){
    if ( i == 0 ) {
      if ( case_gate ) translated[i] = toupper(letter[start]);
      else translated[i] = tolower(letter[start]);
    }
    else{
      if ( start+i >= size ) index = start + i - size;
      else index = start+i;
      translated[i] = tolower(letter[index]);
    }
  }
  translated[size++] = 'a';
  translated[size++] = 'y';
  

}

void translateStream(ifstream& file, ostream& o){
  char character;
  char translated[64];
   
  char word[64];
  int count(0);

  if (file.eof()) return;
  
  while ( file >> noskipws >> character ){
    
    if ( ispunct(character) || isspace(character) ) {
      if ( count != 0 ) {
	word[count] = '\0';
	string english(word); //convert char array to string
	translateWord(english, translated);
	o << translated;
      }
      
      o << character;
      translateStream(file, o);
      return;
    }
      
    word[count] = character;
    count++;
  }  
}

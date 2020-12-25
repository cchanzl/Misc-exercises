#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include <vector>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */
int count_word(const char* word){
  int count(0);
  while ( *(word + count) != 0){
    count++;
  }
  return count;
}

int count_word(const char** word){
  int count(0);
  while ( *(word + count) != 0){
    count++;
  }
  return count;
}

// returns true if it is a valid step
bool valid_step(const char* start, const char* end){
  // assumes both words are in upper case
  
  // Step 1: check if start and end are in dictionary
  if ( !dictionary_search(start) ) return false;
  if ( !dictionary_search(end) ) return false;

  // Step 2: Check if they are the same length
  if( count_word(start) != count_word(end) ) return false;

  // Step 3: Check if there is more than one letter difference
  int count(0);
  for ( int i = 0; i < count_word(start); i++){
    if ( *(start+i) != *(end+i) ) count++;
    if ( count > 1 ) return false;
  }

  // Step 4: check if they are the same word
  if ( count == 0 ) return false;
  
  return true;  
}

bool display_chain(const char** chain, ostream& o){
  int chain_length = count_word(chain);
  int word_length = count_word(chain[0]);
  
  for ( int x = 0; x < chain_length; x++){
    for ( int y = 0; y <  word_length; y++){ 
      if ( x == 0 || x == chain_length - 1 ) o << chain[x][y];
      else o << static_cast<char>(tolower(chain[x][y]));
      }
    o << endl;
  }
  
  return true;

}

// returns true if there is a repeat
bool is_repeat(const char* word, string* sChain, int index){
  int word_length = count_word(word);
  string sWord;
  
  for ( int i = 0; i < word_length; i++){
    sWord.push_back(word[i]);
  }
  sChain[index] = sWord;

  for ( int i = 0; i < index; i++){
    if ( sWord == sChain[i] ) return true;
  }
  
  return false;
  
}

bool valid_chain(const char** chain, int count){
  int chain_length = count_word(chain);
  if ( count != 0 ) chain_length  = count;
  int word_length = count_word(chain[0]);
  string sChain[chain_length];
  
  for ( int i = 0; i < chain_length; i++){
    // Step 1: Check that all words in the chain are the same length
    if ( count_word(chain[i]) != word_length ) return false;

    // Step 2: Compare successive words, to make sure that only 1 word differs
    if ( i == chain_length - 1 && !is_repeat(chain[i], sChain, i)) return true;

    if (is_repeat(chain[i], sChain, i)) return false;

    if ( i < chain_length - 1){
      if ( !valid_step(chain[i], chain[i+1])) {
	//cout << chain[i] << " " << chain[i+1] << endl;
	return false;  
      }
    }
  }
  
  return true;

}

void print_word(const char* word, int length){

  for ( int i = 0; i < length; i++){
    cout << word[i];
  }
  cout << " ";
}

bool is_same_word(const char* start, const char* end){
  for ( int i = 0; i < count_word(start); i++){
    if ( *(start+i) != *(end+i) ) return false;
  }

  if ( count_word(start) != count_word(end)) return false;

  return true;
  
}

bool find_chain(const char* start, const char* end2, const char** chain, int max_steps, int count){
  int start_length = count_word(start);
  int end_length = count_word(end2);
  char* end = new char[end_length];

  for ( int i = 0; i < end_length+1; i++) end[i] = end2[i];
    
  if ( start_length == 0 && end_length == 0 ) return true;
  if ( start_length == 0 || end_length == 0 ) return false;

  // add start word into next index in chain
  if ( count == 0 ){
    chain[count] = start;
    chain[end_length] = end2;
    /*int chain_length = count_word(chain);
    for ( int i = 0; i < chain_length; i++) {
      chain[i] = nullptr;
    }*/
    count++;
  }

  if ( is_same_word(start, end) && valid_chain(chain) ) return true;
  if ( max_steps == 0 && is_same_word(start, end) && valid_chain(chain)) return true; 
  if ( max_steps <= 0 ) return false;
  if ( !valid_chain(chain, count) ) return false;
  
  // initialise next word
  char* next = new char[start_length+1];
  for ( int i = 0; i < start_length+1; i++){
    if ( i == start_length ) {
      next[start_length] = '\0';  // c-string terminated character
      break;
    }
    next[i] = start[i];
  }
  
  // find next word
  for ( int x = 1; x < 26; x++){ // for each alphabet from A - Z
    for ( int y = 0; y < start_length; y++){ // for each letter in the word
      char temp = next[y];
      int next_char;

      if ( next[y] + x > 90) next_char =  next[y] + x - 90 + 64;
      else next_char = next[y] + x;
      //print_word(next, start_length);
      //cout << " " << next_char << " ";

      next[y] = static_cast<char>(next_char); // change alphabet

      //print_word(next, start_length);
      //cout << endl;
      if ( valid_step(start, next) ) {
	chain[count] = next;	
	//print_word(next, start_length);
	/*
	if ( count == 1 )  cout<<"      step: " << count << " " << max_steps << endl;
	else if ( count == 2)  cout << "   step: " << count << " " << max_steps << endl;
	else cout << "step: " << count << " " <<max_steps<< endl;
	*/
	count++;

	if ( find_chain(next, end, chain, max_steps-1, count) ) {
	  //cout << count << endl;
	  return true;
	}

	//cout << chain[count] << endl;
	count--;
      }
      next[y] = temp;
    }
  }

  return false;  
  
}


#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

// returns true if a duplicate is found
bool is_duplicate(string str, string* str_arr){
  while ( *str_arr != "" ) {
    if ( str == *str_arr ) return true;
    str_arr++;
  }
  return false;
}


/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */
char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  static int count(0);
  // the table which maps endings to letters (persists between calls)
  //static map<string, char> lookup;

  static string str_arr[512];;
  static char char_arr[512];
  
  // providing a way to reset the table between poems
  if (ending == RESET) {
    count = 0;
    for ( int i = 0; i < 512; i++){
      str_arr[i] = "";
      char_arr[i] = '\0';
    }
    
    //lookup.clear();    
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  /* if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }*/

  if ( !is_duplicate(end, str_arr) ){
    str_arr[count] = end;
    char_arr[count] = next;
    assert(next <= 'z');
    count++;
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  // return lookup[end];
  for ( int i = 0; i < 512; i++){
    if ( end == str_arr[i] ) return char_arr[i];
  }
}

/* START WRITING YOUR FUNCTION BODIES HERE */

// counts and returns the number of words in a line
int count_words(string line){

  const char* letter = line.c_str();
  char output[512];
   int count(0);

  for ( int i = 1; i < 512; i++){
    if ( get_word(letter, i, output) ) count ++;
    else break;
  }

  return count;
  
}

// returns true if a word has vowels
bool has_vowel(string word){
  
  const char* letter = word.c_str();
  char vowel[5] = {'a', 'e', 'i', 'o', 'u'};

  while ( *letter != '\0'){

    for ( int i = 0; i < 5; i++){
      if ( tolower(*letter) == vowel[i] ) return true;
    }
    letter++;
  }

  return false;

}

// find the phonetic ending for the given word
bool find_phonetic_ending(string word, char* phonetic_ending){

  const char* word2 = word.c_str();
  
  // reset phonetic_ending array
  for ( int i = 0; i < 512; i++) phonetic_ending[i] = '\0';

  // search dictionary.txt
  string line;
  string phonetic;
  ifstream in("dictionary.txt");

  // find word in dictionary and extract the corresponding set of phonemes
  bool found = false;
  while(getline(in, line)){
    if ( line.find(word, 0) != string::npos) {
      char output[512];
      const char* line2 = line.c_str();
 
      get_word(line2, 1, output);
      if ( !strcmp(output, word2) ){ // convert string to C-string
	phonetic = line; // extracting phonemes here
	found = true;
	break;
      }
    }
  }

  // return false if word is not found in dictionary
  if ( !found ) return false;
  
  // get count of words
  int count = count_words(phonetic);

  int index(0);
  const char* phonetic_char = phonetic.c_str();

  // check which word contains the first vowel in the set of phonemes
  for ( int i = 2; i <= count; i++){
    char phonemes[512];
    get_word(phonetic_char, i, phonemes);
    if ( has_vowel(phonemes) ) index = i; 
  }

  // concatenate the from index(th) word onwards and add to phonetic_ending
  for ( int i = index; i <= count; i++){
    char phonemes[512];
    get_word(phonetic_char, i, phonemes);
    strcat(phonetic_ending, phonemes);
  }
  return true;
}

// find the rhyme scheme for a given sonnet
bool find_rhyme_scheme(const char* filename, char* scheme){

  // open input file
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "Error: Unable to open file" << std::endl;
    return false;
  }

  // reset for a new poem
  rhyming_letter(RESET);

  // read each line in file and retrieve last word, phonetic_ending and hence rhyme
  string line;
  while(getline(in, line)){
    int count = count_words(line);
    const char* line2 = line.c_str();
 
    char last_word[512];
    if ( !get_word(line2, count, last_word) ) continue;
    
    char phonetic_ending[512];
    if ( !find_phonetic_ending(last_word, phonetic_ending) ) continue;

    *scheme = rhyming_letter(phonetic_ending);
    scheme++;
  }

  in.close(); // close file
  
  return true;
}

// compare the sonnet in the file with known rhyme scheme 
string identify_sonnet(const char* filename){

  char scheme[512];

  // return Unknown if cannot open file
  if ( !find_rhyme_scheme(filename, scheme) ) return "Unknown"; 

  const char shakespearean[]{"ababcdcdefefgg"};
  string petrarch = "abbaabbacdcdcd";
  string spenserian = "ababbcbccdcdee";

  if ( !strcmp(scheme, shakespearean) ) return "Shakespearean";
  if ( scheme == petrarch ) return "Petrarchan";
  if ( scheme == spenserian ) return "Spenserian";

  // return Unknown if cannot find rhyme scheme
  return "Unknown";

}


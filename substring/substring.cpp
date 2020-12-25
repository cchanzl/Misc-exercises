#include <iostream>

using namespace std;

// ----------- Helper Function ----------

int count_str(const char* word){

  int count(0);
 
  while(*word != '\0'){
    count++;
    word++;
  }

  return count;
}

string copy_str(const string original, int start, const int end){
  string copied = "";
  int count(0);
  int max = end - start + 1;
  
  while(count != max){
    copied = copied + original[start];
    count++;
    start++;
  }
  return copied;
  
}

// ---------- Main Functions ----------
// only compares from position 0 onwards
bool is_prefix(string start, string end){

  const char* startPtr = start.c_str();
  const char* endPtr = end.c_str();
  
  int prefix_l = count_str(startPtr);
  int word_l = count_str(endPtr);

  if ( prefix_l > word_l ) return false;

  if ( prefix_l == 0 ) return true;
  
  if ( *endPtr == *startPtr ){
    string copy_start = copy_str(start, 1, prefix_l);
    string copy_end = copy_str(end, 1, word_l);
    if ( is_prefix(copy_start, copy_end) ) return true;
  }

  return false;
  
}


int substring_position(const string prefix, const string word, int index = 0){

  const char* startPtr = prefix.c_str();
  const char* endPtr = word.c_str();
 
  int prefix_l = count_str(startPtr);
  int word_l = count_str(endPtr);

  if ( prefix_l == 0 ) return 0;
  if ( prefix_l > word_l ) return -1;

  if ( *startPtr == *endPtr ) {
    string copy_start = copy_str(prefix, 1, prefix_l);
    string copy_end = copy_str(word, 1, word_l);
    if ( is_prefix(copy_start, copy_end) ) return index;
  }
  
  index++;
  string copy_end = copy_str(word, 1, word_l);
  int position =  substring_position(prefix, copy_end, index);
  if ( position >= 0 ) return position;
 
  return -1;
}

#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;

void reverse(string str1, char str2[]){

  const char* letter = str1.c_str();
  int length = static_cast<int>(str1.length());
  int count(0);
  char* temp = str2;
  
  // Erase str2
  while ( *str2 != '\0' ){
    *str2 = '\0';
    str2++;
  }

  str2 = temp;

  // reversing word
  while( count < length ){
    str2[count] = *(letter + length - count - 1);
    count++;
  }
 

}

string copy_str(const string original, int start, const int end){
  string copied = "";

  while(original[start] != '\0'){
    copied = copied + original[start];
    start++;
  }

  return copied;
  
}

bool end_string(const char* str){
  if ( *str == '\0' ) return true;

  while ( *str != '\0' ){
    if ( isalpha(*str) ) return false;
    str++;
  }
  return true;

}

bool compare(string one, string two){
  const char* oneC = one.c_str();
  const char* twoC = two.c_str();
  
  int one_l = static_cast<int>(one.length());
  int two_l = static_cast<int>(two.length());
  //cout << endl;
  //cout << one << endl;
  //cout << two << endl;
  // make a copy of one and two without the first character
  string copy_one = copy_str(one, 1, one_l);
  string copy_two = copy_str(two, 1, two_l);

  // returns true if reached end of string
  if ( *oneC == '\0' ) return end_string(twoC);
  if ( *twoC == '\0' ) return end_string(oneC);
  
  // if the char is a punctuation, call compare again
  if ( !isalpha(*oneC) ) return compare(copy_one, two);
  if ( !isalpha(*twoC) ) return compare(one, copy_two);

  // if it is alpha and not the same, return false
  if ( toupper(*oneC) != toupper(*twoC) ) return false;
  
  // one equals two after the checks above
  return compare(copy_one, copy_two); 
}

bool palindrome(string sentence){

  int length = static_cast<int>(sentence.length());
  char palindrome[length+1];
  for ( int i = 0; i < length+1; i++){
    if ( i == length ) palindrome[length] = '\0';
    palindrome[i] = 0;
  }

  reverse(sentence, palindrome);
  return compare(sentence, palindrome);  
}

// sort from A to Z
string sort_str(string str){
  string copied = "";
  int length = static_cast<int>(str.length());
  const char* strPtr = str.c_str();
  vector<char> str_copy;
  
  if ( length == 0 ) return copied;
  
  for ( int i = 0; i < length; i++ ){
    if ( !isalpha(*strPtr) ) {
      strPtr++;
      continue;
    }

    char alphabet = toupper(*strPtr);

    str_copy.push_back(alphabet);
    strPtr++;
    continue;
  }

  std::sort(str_copy.begin(), str_copy.end());
  // convert str_copy to string
  std::string strr(str_copy.begin(), str_copy.end());
  return strr;
}

bool anagram(string str1, string str2){
  string sorted_str1 = sort_str(str1);
  string sorted_str2 = sort_str(str2);
  /*
  cout << str1 << endl;
  cout << sorted_str1 << endl;
  cout << str2 << endl;
  cout << sorted_str2 << endl;
  */
  return compare(sorted_str1, sorted_str2);
}

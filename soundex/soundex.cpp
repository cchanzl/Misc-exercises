#include <iostream>
#include <cctype>

using namespace std;

void encode(string surname, char soundex[]){
  int count(0);
  const char* letter = surname.c_str();
  int length = static_cast<int>(surname.length()) -1;
  
  // Erase soundex
  for ( int i = 0; i < 5; i++) soundex[i] = 0;
  
  
  if ( surname.length() == 0 ) return;

  soundex[0] = toupper(surname[0]);
  letter++;

  char digits[length];
  
  while(count < length ){
 
    if ( !isalpha(*letter)){
      //cout << *letter << endl;
      letter++;
      continue;
    }
    
    char c = tolower(*letter);
    
    switch(c){
    case 'b': case 'f': case 'p': case 'v':
      digits[count] = '1';
      break;

    case 'c': case 'g': case 'j': case 'k': case 'q': case 's': case 'x': case 'z':
      digits[count] = '2';
      break;
           
    case 'd': case 't':
      digits[count] = '3';
      break;     
      
    case 'l':
      digits[count] = '4';
      break;

    case 'm': case 'n':
      digits[count] = '5';
      break;

    case 'r':
      digits[count] = '6';
      break;

    default:
      digits[count] = '0';
      break;
    }
    count++;
    letter++;
  }

  //  cout << digits << " " << length << endl;

  count = 1;
  for ( int i = 0; i < length; i++){
    if ( count > 3 ) break;
    if( digits[i] == '0' ) continue;
    if (  digits[i] != '0' && i == 0 ){
      soundex[count] = digits[i];
      count++;
      continue;
    }

    if ( digits[i] == digits[i-1] ) continue;

    soundex[count] = digits[i];
    count++;
  }
  
  for ( int i = count; i < 4; i++) soundex[i] = '0';
    
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


bool compare(string one, string two){
  //if( one.compare(two) != 0 ) return false;
  //return true;

  if ( one[0] == '\0' && two[0] == '\0' ) return true;
  
  if ( one[0] == two[0] ){
    string copy_one = copy_str(one, 1, 4);
    string copy_two = copy_str(two, 1, 4);
    if ( compare(copy_one, copy_two) ) return true;
  }

  return false;
  
}

int count(string surname, string sentance){
  char surname_soundex[5];
  char soundex[5];
  encode(surname, surname_soundex);

  int count(0);
  const char* letter = sentance.c_str();
  string word = "";
  
  // create words
  while ( *letter != '\0'){

    if ( !isalpha(*letter) ){
      encode(word, soundex);
      //cout << word << " " << soundex << endl;
      if ( compare(soundex, surname_soundex) ) count++;
      letter++;
      word = "";
      continue;
    }

    word.push_back(*letter);
    letter++;

  }

  return count;
  
}


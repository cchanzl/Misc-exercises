#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */

// returns number of leading zeroes. -1 if invalid
int leading_zeros(std::string digest){
  const char* digested = digest.c_str();
  int zero_count(0);
  bool non_zero = false; // indicator that a first non-zero is met in front
  
  while (*digested != '\0'){

    // check individual character
    bool valid = false;
    for ( char i = '0'; i <= '9'; i++){
      if ( i == *digested ) valid = true; 
    }

    for ( char i = 'a'; i <= 'z'; i++){
      if ( i == *digested ) valid = true; 
    }

    if ( !valid ) return -1; // return if invalid character

    // count zeroes
    if ( *digested == '0' && !non_zero ) zero_count += 1;
    if ( *digested != '0' ) non_zero = true;
    
    digested++;
  }
  return zero_count;
}

// return true if SHA1 digest of the contents can be copied. false if otherwise
bool file_to_SHA1_digest(const std::string filename, char* digest){

  // opening file
  std::ifstream in(filename);
  if (!in) return false;
 
  // count char in file
  string line;
  int count(0);
  while( getline ( in, line ) ){
    count += line.length();
  }

  // go to beginning of file
  in.clear();
  in.seekg(0);
  
  // get message
  char message[count];
  while( getline(in, line) ){
    const char* line2 = line.c_str();
    strcat(message, line2);
  }
  text_to_SHA1_digest(message, digest);
  
  // close file
  in.close();

  return true;

}

bool make_front(const std::string recipient, const std::string filename, char* header){
  
  // add recipient to header
  int index(0);
  int count(0);
  while ( recipient[index] != '\0' ){
    *header = recipient[index];
    index++;
    header++;
    if ( recipient[index] == '@' ) count++;
  }
 
  if ( count != 1 ) return false; // invalid email address

  // add colon
  *header = ':';
  header++;

  // add digest of message
  int size = 41;
  char digest[size];
  // for ( int i = 0; i < size; i++) digest[i] = '\0';
  strcpy(digest, "");
  if ( !file_to_SHA1_digest(filename, digest) ) return false; // invalid message
  
  index = 0; // reset index
  while( digest[index] != '\0' ) {
    *header = digest[index];
    index++;
    header++;
  }
  
  // add colon
  *header = ':';
  header++;

  return true;
}

// returns false if file cant be read or counter exceeds 10 mn. return true otherwise
bool make_header(const std::string recipient, const std::string filename, char* header){
  char* temp = header;
  
  // flush header 
  for ( int i = 0; i < 512; i++ ) header[i] = '\0';
  
  // add email and message digest, only for the first run
  if ( !make_front(recipient, filename, header) ) return false;
  
  // find last semi colon
  int count(0);
  while ( count != 2 ){
    if ( *header == ':' ) count++;
    header++;
  }
    
  // add counter
  int counter(0);
  int size = 41;
  char digest2[size];
  char* temp2 = header;

  // loop through each counter
  while ( counter < 10000000 ){
    header = temp2;
    string counter_S = to_string(counter);
    const char* counter_C = counter_S.c_str();

    while ( *counter_C != '\0' ){
      *header = *counter_C;
      counter_C++;
      header++;
    }
    
    //cout << temp << endl;

    // convert header to SHA1
    for ( int i = 0; i < size; i++) digest2[i] = '\0';
  
    text_to_SHA1_digest(temp, digest2);
    int num_zeros =  leading_zeros(digest2);
    if ( num_zeros == 5 ) return true;
    counter++;
  }
  
  return false;
}

// check message received 
MessageStatus check_header(std::string email_address,
			   std::string header, std::string filename){

  // find position of colon
  int index(0);
  int position1(0);
  int position2(0);
  int count_colon(0);
  while ( header[index] != '\0' ){
    if ( header[index] == ':' ) count_colon++;
    if ( header[index] == ':' && position1 == 0 )position1 = index;
    if ( header[index] == ':' && position1 != 0 && position2 == 0 ) position2 = index;
    index++;
  }
  if ( count_colon != 2 ) return INVALID_HEADER;

  // check email address
  index = 0;
  while ( header[index] != ':' ){
    if ( header[index] != email_address[index] ) return WRONG_RECIPIENT;
    index++;
  }

  // check message digest
  index = position1 + 1;
  int size = 41;
  char digest[size];
  for ( int i = 0; i < size; i++) digest[i] = '\0';
  int count(0);
  while ( header[index] != ':' ){
    digest[count] = header[index]; // obtain message digest
    count++;
    index++;
  }

  char digest2[size];
  if ( !file_to_SHA1_digest(filename, digest2) ) return INVALID_MESSAGE_DIGEST;

  //cout << digest << endl;
  //cout << digest2 << endl;
  
  for ( int i = 0; i < size; i++){
    if ( digest[i] != digest2[i] ) return INVALID_MESSAGE_DIGEST;
  }

  // check counter in header digest
  char head[512];
  if ( !make_header(email_address, filename, head) ) return INVALID_HEADER_DIGEST;
  index = position2 + 1;
  while ( header[index] != '\0' ){
    if ( header[index] != head[index] ) return INVALID_HEADER;
    index++;
  }
  return VALID_EMAIL;
}

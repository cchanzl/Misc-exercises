#include <iostream>
#include <string>
#include <cstring>

using namespace std;

// produce output suitable for Playfair encoding
void prepare(string input, char* output){

  const char* letter = input.c_str();
  int count(0);

  // exit loop when null terminating char is reached
  while( *letter != '\0'){
    // cout << *letter << endl;
    if ( isalnum(*letter) != 0 ) {
      *output = toupper(*letter); 
      count++;
      output++;
    }
    letter++;
  }
  
  if ( count % 2 != 0 ) {
    *output = 'X';
    output++;
  }
  *output = '\0';
}

// Prepare grid for Playfair encoding based on a codeword
void grid(string codeword, char square[6][6]){

  const char* letter = codeword.c_str();
  int length = codeword.length();
  int row(0); // keep track of number of sqaures populated
  int col(0);
  int square_length = 6;
  
  // prepare an array of 26 + 10 char to track usage
  // alphabets for from 0 to 25 and numeric goes from 26 to 35
  int total_char = square_length * square_length;
  int alphabet[total_char];
  for ( int i = 0; i < total_char; i++) alphabet[i] = i+1;

  // reset square[6][6]
  for ( int r = 0; r < square_length; r++){
    for ( int c = 0; c < square_length; c++){
      square[r][c] = '\0';
    }
  }

  int index(0); 
  // add codeword
  for ( int i = 0; i < length; i++){
    // convert char to array index
    // assume codeword consists of uppercase and digits only
    if ( isalpha(*letter) ) index = static_cast<int>(*letter) - 'A';
    else index = static_cast<int>(*letter) - '0' + 25;

    // continue if it has already been used
    if ( alphabet[index] < 0 ) {
      letter++; // go to next letter
      continue;
    }
      
    // add if it has not been used
    alphabet[index] = -alphabet[index];
    square[row][col] = *letter;
    //cout << row << col << " " << *letter << " " << square[row][col] <<  endl;
    
    // incrementing cells in square
    if ( col == square_length - 1 ) { // if last col
      row++;
      col = 0;
    }
    
    else col++;
    letter++;
  }
 
  // complete the remaining square
  for ( int r = 0; r < square_length; r++){
    for ( int c = 0; c < square_length; c++){

      if ( square[r][c] != '\0' ) continue;

      // find next codeword that is not used
      for ( int i = 0; i < total_char; i++){
	if ( alphabet[i] > 0 ) {
	  // if alphabet, else numeric
	  if ( i < 26 ) square[r][c] = static_cast<char>(alphabet[i] + 'A' - 1);
	  else square[r][c] = static_cast<char>(i + 22);
	  alphabet[i] = -alphabet[i];
	  break;
	}
      }      
    }
  } 
  
}

// helper function to obtain coordinate in Playfair grid
void obtain_rc(char square[6][6], const char inchar, int& row, int& col){

  // find coordinates for inchar1 and inchar2
  for ( int r = 0; r < 6; r++){
    for ( int c = 0; c < 6; c++){
      if ( square[r][c] == inchar ){
	row = r;
	col = c;
	return;
      }
    }
  }

}

// Function to encode a single bigram (two letter pair)
void bigram(char square[6][6], char inchar1, char inchar2, char& outchar1, char& outchar2){

  int row1(0);
  int col1(0);
  int row2(0);
  int col2(0);

  // check for alpha numeric
  if ( !isalnum(inchar1) || !isalnum(inchar2) ) {
    cerr << inchar1 << " or " << inchar2 << " is not alphanumeric!" << endl;
    return;
  }
    
  // find coordinates for inchar1 and inchar2
  obtain_rc(square, inchar1, row1, col1);
  obtain_rc(square, inchar2, row2, col2);
  
  // output resulting char
  outchar1 = square[row1][col2];
  outchar2 = square[row2][col1]; 
}

// Function to encode a prepared input string using a given encoding grid
void encode(char square[6][6], string prepared, char* encoded, int count){

  const char* letter = prepared.c_str();
  int no_of_letters(2);
  
  // stop recursion when null terminating character is reached
  if ( *letter == '\0' ) return;
  
  // reset encoded array
  if ( count == 0 ) {
    for ( int i = 0; i < 100; i++ ) encoded[i] = '\0';
  }
  
  // encode current two char
  bigram(square, *letter, *(letter+1), *encoded, *(encoded+1));

  // encode next two char
  encode(square, letter + no_of_letters, encoded + no_of_letters, 1); 

}

// Function to decode a Playfair encoded message
void decode(char square[6][6], string encoded, char* decoded, int count){
  
  const char* letter = encoded.c_str();
  
  // stop recursion when null terminating character is reached
  if ( *letter == '\0' ) return;
  
  // reset decoded array
  if ( count == 0 ) {
    for ( int i = 0; i < 100; i++ ) decoded[i] = '\0';
  }

  int row1(0);
  int col1(0);
  int row2(0);
  int col2(0);

  obtain_rc(square, *letter, row1, col1);
  obtain_rc(square, *(letter+1), row2, col2);
  
  // decode current two char
  if ( col1 == col2 ) bigram(square, *letter, *(letter+1), *decoded, *(decoded+1));
  else bigram(square, *(letter+1), *letter, *decoded, *(decoded+1));

  // decode next two char
  encode(square, letter+2, decoded+2, 1); 
  
}


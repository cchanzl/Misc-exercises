#include <iostream>
#include <string>
#include <cstring>

using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary);

/* converts a binary string representation into a character */
char binary_to_ascii(char *binary);

void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

char binary_to_ascii(char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */

void text_to_binary(string str, char* encoded, int count){

  const char* alphabet = str.c_str();

  // break condition
  if ( *alphabet == '\0' ) return;
  
  // clear encoded array
  if ( count == 0){
    for ( int i = 0; i < 512; i++) encoded[i] = '\0';
  }
  
  // encoding
  ascii_to_binary(*alphabet, encoded);

  // next loop
  text_to_binary(alphabet+1, encoded+8, count+8);
  
}

void binary_to_text(string str, char* text, int count){
  
  const char* binary = str.c_str();

  // break condition
  if ( *binary == '\0' ) return;
  
  // clear encoded array
  if ( count == 0){
    for ( int i = 0; i < 32; i++) text[i] = '\0';
  }

  // encoding
  char bin[8];
  for ( int i = 0; i < 8; i++){
    bin[i] = *(binary+i);
    //cout << bin[i];
  }
  //cout << endl;

  //cout << binary_to_ascii(bin) << endl;
  text[count] = binary_to_ascii(bin);
  count++;
  
  // next loop
  binary_to_text(binary+8, text, count);  

}

char parity(char c1, char c2, char c3){
  int one = static_cast<int>(c1) - 48;
  int two = static_cast<int>(c2) - 48;
  int three = static_cast<int>(c3) - 48;
  //  cout << one << two << three << endl;
  int sum = one + two + three;

  if ( sum % 2 == 0 ) return '0';
  return '1';  
}

void add_error_correction(string data, char* corrected, int count){

  const char* binary = data.c_str();
  
  // break condition
  if ( *binary == '\0' ) return;
    
  // clear correct array
  if ( count == 0){
    for ( int i = 0; i < 512; i++) corrected[i] = '\0';
  }

  // retrieving 4 bit
  char bin[4];
  for ( int i = 0; i < 4; i++){
    bin[i] = *(binary+i);
  }

  // calculate parity
  char c1 = parity(bin[0], bin[1], bin[3]);  
  char c2 = parity(bin[0], bin[2], bin[3]);
  char c3 = parity(bin[1], bin[2], bin[3]);

  // encoding error corection information
  corrected[count+0] = c1;
  corrected[count+1] = c2;
  corrected[count+2] = bin[0];
  corrected[count+3] = c3;
  corrected[count+4] = bin[1];
  corrected[count+5] = bin[2];
  corrected[count+6] = bin[3];
  //cout << corrected[count+0] << corrected[count+1] << corrected[count+2];
  //cout << corrected[count+3] << corrected[count+4] << corrected[count+5];
  //cout << corrected[count+6] << endl;

  add_error_correction(binary+4, corrected, count+7);
     
}


char parity(char c1, char c2, char c3, char c4){
  int one = static_cast<int>(c1) - 48;
  int two = static_cast<int>(c2) - 48;
  int three = static_cast<int>(c3) - 48;
  int four = static_cast<int>(c4) - 48;

  //  cout << one << two << three << endl;
  int sum = one + two + three + four;

  if ( sum % 2 == 0 ) return '0';
  return '1';  
}



int decode(string received, char* decoded, int error, int count){
  const char* binary = received.c_str();

  // break condition
  if ( *binary == '\0' ) return error;
  
  // clear correct array
  if ( count == 0) for ( int i = 0; i < 512; i++) decoded[i] = '\0';
  
  // retrieving 7 bit
  char bin[7];
  for ( int i = 0; i < 7; i++) bin[i] = *(binary+i);

  // calculate parity
  char p1 = parity(bin[3], bin[4], bin[5], bin[6]);  
  char p2 = parity(bin[1], bin[2], bin[5], bin[6]);
  char p3 = parity(bin[0], bin[2], bin[4], bin[6]);

  // if errors are present
  if ( p1 == '1' || p2 == '1' || p3 == '1') {

    // convert position to integers
    int one = static_cast<int>(p1) - 48;
    int two = static_cast<int>(p2) - 48;
    int three = static_cast<int>(p3) - 48;
    int position = one*(4) + two*(2) + three; 
    // cout << one << two << three << " " << position << endl;
    if ( bin[position-1] == '1' ) bin[position-1] = '0';
    else bin[position-1] = '1';
    error++;
  }
 
  // if no errors or after correcting errors
  decoded[count+0] = bin[2];
  decoded[count+1] = bin[4];
  decoded[count+2] = bin[5];
  decoded[count+3] = bin[6]; 

  //cout << bin[0] << bin[1] << bin[2] << bin[3];
  //cout << bin[4] << bin[5] << bin[6] << bin[7] << endl;
  //cout << bin[2] << bin[4] << bin[5] << bin[6] << endl;;
  
  return decode(binary+7, decoded, error, count+4);

  
}

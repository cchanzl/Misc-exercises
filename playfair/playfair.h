#ifndef PLAYFAIR_H
#define PLAYFAIR_H

using namespace std;

void prepare(string input, char* output);
void grid(string codeword, char square[6][6] );
void bigram(char square[6][6], char inchar1, char inchar2, char& outchar1, char& outchar2);
void encode(char square[6][6], string prepared, char* encoded, int count = 0);
void decode(char square[6][6], string encoded, char* decoded, int count = 0);



#endif

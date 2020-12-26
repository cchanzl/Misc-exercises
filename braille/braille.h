#ifndef BRAILLE_H
#define BRAILLE_H

using namespace std;

int encode_character(char ch, char braille[], int start = 0);
void encode(string plaintext, char braille[], int count = 0);
void print_braille(string plaintext, ostream& o);

#endif

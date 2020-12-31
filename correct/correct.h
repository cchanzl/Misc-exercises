#ifndef CORRECT_H
#define CORRECT_H

using namespace std;

void ascii_to_binary(char letter, char *output);
char binary_to_ascii(char *binary);
void text_to_binary(string str, char* encoded, int count = 0);
void binary_to_text(string str, char* text, int count = 0);
void add_error_correction(string data, char* corrected, int count = 0);
int decode(string received, char* decoded, int error = 0, int count = 0);

#endif

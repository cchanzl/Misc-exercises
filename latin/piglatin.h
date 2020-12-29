#ifndef LATIN_H
#define LATIN_H

using namespace std;

int findFirstVowel(string word);
void translateWord(string english, char translated[]);
void translateStream(ifstream& file, ostream& o);

#endif

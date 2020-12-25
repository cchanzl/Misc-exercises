#ifndef SOUNDEX_H
#define SOUNDEX_H

using namespace std;

void encode(string surname, char soundex[]);
bool compare(string one, string two);
int count(string surname, string sentance);


#endif

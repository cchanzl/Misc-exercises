/*  This pre-supplied function looks up a given word in the dictionary
    of approved words. It is assumed that the input word is in uppercase.
    The function returns true if the word is in the dictionary.
    Otherwise returns false. */
#include <ostream>

bool dictionary_search(const char *word);

/* add your function prototypes here */
bool valid_step(const char* start, const char* end);
bool display_chain(const char** chain, ostream& o);
bool valid_chain(const char** chain);
bool find_chain(const char* start, const char* end, const char** chain, int max_steps);

int count_word(const char* word);
int count_word(const char** word);

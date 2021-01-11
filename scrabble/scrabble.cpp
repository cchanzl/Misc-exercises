#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */

int tile_score(const char tile){

  // if tile is blank or ?
  if ( tile == ' ' || tile == '?' ) return 0;

  int score [] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};
  char offset = 'A';
  if ( islower(tile) ) offset = 'a';
  if ( isalpha(tile) ){
    int index = static_cast<int>(tile) - offset;
    return score[index];
  }
  
  return -1;
}

bool can_form_word_from_tiles(const char* word, const char* tiles,
			      char* played_tiles, int* track_tiles, int count){

  // end recursion
  if ( *word == '\0' ) {
    delete[] track_tiles; // deallocate
    return true;
  }

  // find length of word
  int length = strlen(tiles);

  // declare on the heap an index to track used tiles on first call of this function
  if ( count == 0 ) {
    track_tiles = new int[length];
    for ( int i = 0; i < length; i++) track_tiles[i] = 1;

    // flush played_tiles 
    for ( int i = 0; i < 80; i++) played_tiles[i] = '\0';
  }
  
  // check if can find the first char in word
  int position(0);
  const char* temp = tiles;
  while ( *temp != '\0' ){
    //cout << *word << " " << *temp << endl;
    
    if ( *word == *temp && track_tiles[position] > 0) {
      // if not used, mark as used and call function again
      track_tiles[position] = -track_tiles[position]; 
      played_tiles[count] = *word; // save letter
      count++; // next position in played_tiles
      word++; // next word
      return can_form_word_from_tiles(word, tiles, played_tiles, track_tiles, count);
    }
    temp++; // next tile
    position++; // next index
  }

  // if cannot find available letter, check if '?' is available
  const char* temp2 = tiles;
  position = 0;
  while ( *temp2 != '\0' ){
    if ( *temp2 == '?' && track_tiles[position] > 0 ){
      // if not used, mark as used and call function again
      track_tiles[position] = -track_tiles[position]; 
      played_tiles[count] = '?';
      count++; // next position in played_tiles
      word++; // next word
      return can_form_word_from_tiles(word, tiles, played_tiles, track_tiles, count);
    }
    temp2++;
    position++;
  }

  // if cannot find available alphabet or '?', deallocate and return false
  delete[] track_tiles; // deallocate
  return false;  
}

int compute_score(const char* played_tiles, ScoreModifier* score_modifiers){

  int score(0);
  int count(0);
  bool double_word = false;
  bool triple_word = false;

  // sum score from individual tiles first
  while ( *played_tiles != '\0' ){
    int multiplier(1);

    // here i am assuming double/triple letter applies even if it is a '?' tile
    // trvial to update if condition to reflect otherwise
    if ( score_modifiers[count] == DOUBLE_WORD_SCORE ) double_word = true;
    if ( score_modifiers[count] == TRIPLE_WORD_SCORE ) triple_word = true;

    if ( score_modifiers[count] == DOUBLE_LETTER_SCORE ) multiplier = 2;
    if ( score_modifiers[count] == TRIPLE_LETTER_SCORE ) multiplier = 3;

    // compute score
    score += tile_score(*played_tiles) * multiplier;  

    //cout << *played_tiles << tile_score(*played_tiles) << " " <<  multiplier;
    //cout << " = " << score << endl;

    played_tiles++; // next tile
    count++;
  }

  // double / triple word. assume mutually exclusive
  if ( triple_word ) score *= 3;
  else if ( double_word ) score *= 2; 

  // check if all 7 tiles are used
  if ( count == 7 ) score += 50;
  
  return score;
}

// returns the highest scoring combination of word
int highest_scoring_word_from_tiles(const char* tiles, ScoreModifier* score_modifier,
				    char* word){

  // flush word
  for ( int i = 0; i < 512; i++ ) word[i] = '\0';

  // open file
  std::ifstream in("words.txt");
  if (!in) {
    in.close();
    return -1;
  }

  // initialise variables
  string line;
  char played_tiles[80];
  int max_score(0);

  // get each word in dictionary
  while ( getline(in, line) ){
    // flush played_tiles on new line
    for ( int i = 0; i < 80; i++) played_tiles[i] = '\0';

    // convert string to const char*
    const char* dictionary_word = line.c_str();

    // compute and compare score, if a word can be formed
    if ( can_form_word_from_tiles(dictionary_word, tiles, played_tiles) ){
      int score = compute_score(played_tiles, score_modifier);

      // update max_score and best word 
      if ( score > max_score ) {
	max_score = score;
	strcpy(word, dictionary_word);
      }
    }    
  }

  if ( max_score == 0 ) return -1;

  in.close();
  
  return max_score;

}

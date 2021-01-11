enum ScoreModifier { NONE, DOUBLE_LETTER_SCORE, TRIPLE_LETTER_SCORE, DOUBLE_WORD_SCORE, TRIPLE_WORD_SCORE };

/* insert your function prototypes here */

// returns score of a single tile
int tile_score(const char tile);

// returns true if word can be formed from tiles given. false otherwise
bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles, int* track_tiles = nullptr, int count = 0);

// returns the score from a set of played tiles
int compute_score(const char* played_tiles, ScoreModifier* score_modifiers);

// returns the highest scoring combination of word. -1 if not possible to form any word.
int highest_scoring_word_from_tiles(const char* tiles, ScoreModifier* score_modifier,
				    char* word);

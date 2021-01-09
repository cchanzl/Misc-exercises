enum MoveResult { INPUT_ERROR = -4, INVALID_MOVE=-3, REDUNDANT_MOVE=-2, BLOWN_UP=-1, SOLVED_BOARD=1, VALID_MOVE=0 };

// define global constants
#define BOARD_LENGTH    9   // length of board
#define AROUND_SQUARE   8   // number of squares around a target square

// provided functions
void load_board(const char *filename, char board[9][9]);
void display_board(const char board[9][9]);
void initialise_board(char board[9][9]);

// added functions
bool is_complete(char mines[9][9], char revealed[9][9]);
int count_mines(std::string position, char mines[9][9]);
MoveResult make_move(std::string position, char mines[9][9], char revealed[9][9], int count = 0);
bool find_safe_move(char revealed[9][9], char* move);
void solve_board(char mines[9][9], char revealed[9][9], char* moves, int index = 0);

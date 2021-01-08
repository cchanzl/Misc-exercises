#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */

bool is_complete(char mine[9][9], char revealed[9][9]){
  // check that all mines are marked correctly in revealed
  for ( int r = 0; r < 9; r++){
    for ( int c = 0; c < 9; c++){
      if ( mine[r][c] == '*' && revealed[r][c] != '*' ) return false;
    }
  }

  // check that there is no '?' left in revealed
  for ( int r = 0; r < 9; r++){
    for ( int c = 0; c < 9; c++){
      if ( revealed[r][c] == '?' ) return false;
    }
  }

  return true;
  
}

// return number of appearance a char appears around a cell
int count_around(char board[9][9], int row, int col, char target){

  // create coordinates
  int top_left[2] = {row-1, col-1};
  int top[2] = {row-1, col};
  int top_right[2] = {row-1, col+1};
  int right[2] = {row, col+1};
  int bot_right[2] = {row+1, col+1};
  int bottom[2] = {row+1, col};
  int bot_left[2] = {row+1, col-1};
  int left[2] = {row, col-1};

  int* around[8] = {top_left, top, top_right, right, bot_right, bottom, bot_left, left};
  int count(0);  

  // search for mines
  for ( int i = 0; i < 8; i++){
    if(around[i][0]>=0 && around[i][0]<=8 && around[i][1] >= 0 && around[i][1] <= 8){
      if ( board[around[i][0]][around[i][1]] == target ) count += 1;
    }
  }
  return count;
}

int count_mines(string position, char mines[9][9]){
  // error checking
  if ( position.length() != 2 ) return -4;
  if ( !isalpha(position[0]) || !isupper(position[0]) ) return -4;
  if ( !isdigit(position[1]) ) return -4;

  int row = static_cast<int>(position[0]) - 'A';
  int col = static_cast<int>(position[1]) - '0' - 1;
  int mine_count = count_around(mines, row, col, '*');

  return mine_count;
}

MoveResult make_move(std::string position, char mines[9][9], char revealed[9][9], int count){
  // error checking
  int length = position.length();
  if ( !isalpha(position[0]) || !isupper(position[0]) ) return INPUT_ERROR;
  if ( !isdigit(position[1]) ) return INPUT_ERROR;
  if ( length != 2 && length != 3 ) return INPUT_ERROR;

  // convert to integer coordinates
  int row, col;
  if ( length >= 2 ){
    row = static_cast<int>(position[0]) - 'A';
    col = static_cast<int>(position[1]) - '0' - 1;
  }
  if ( row < 0 || row > 8 || col < 0 || col > 8 ) return INVALID_MOVE; 

  // check string last position
  if ( length == 3 && position[2] != '*' ) return INVALID_MOVE;

  // highlight mine
  if ( length == 3 ){
    if ( revealed[row][col] != '?' ) return INVALID_MOVE;
    revealed[row][col] = '*';
    return VALID_MOVE;
  }

  //// to reveal from here onwards
  
  // check if target is uncovered or already flagged
  if ( isdigit(revealed[row][col]) || revealed[row][col] == '*' || revealed[row][col] == ' ' ) return REDUNDANT_MOVE;

  // blown up condition
  if ( count != 1 && revealed[row][col] == '?' && mines[row][col] == '*' ) return BLOWN_UP;

  // if there are adjacent mines
  int mine_count = count_mines(position, mines);
  if ( mine_count > 0 ){
    revealed[row][col] = static_cast<char>(mine_count + '0');
  }
  
  // if no adjacent mines, and uncover adjacent blank squares recursively
  if ( mine_count == 0 ){
    int top_left[2] = {row-1, col-1};
    int top[2] = {row-1, col};
    int top_right[2] = {row-1, col+1};
    int right[2] = {row, col+1};
    int bot_right[2] = {row+1, col+1};
    int bottom[2] = {row+1, col};
    int bot_left[2] = {row+1, col-1};
    int left[2] = {row, col-1};

    int* around[8] = {top_left, top, top_right, right, bot_right, bottom, bot_left, left};

    revealed[row][col] = ' ';
    //cout << position << " with this many mines: " << mine_count << endl;
  
    for ( int i = 0; i < 8; i++){
      if(around[i][0]>=0 && around[i][0]<=8 && around[i][1] >= 0 && around[i][1] <= 8){
	if ( revealed[around[i][0]][around[i][1]] == '?' ) {
	  string pos;
	  pos.push_back(static_cast<char>(around[i][0] + 'A'));
	  pos.push_back(static_cast<char>(around[i][1] + '0' + 1));
	  make_move(pos, mines, revealed, 1);
	}
      }
    }   
  }
  
  // solved board
  if ( is_complete(mines, revealed) ) return SOLVED_BOARD;

  // return valid move
  return VALID_MOVE;

}

// updates row and col with the first empty cell found
void find_empty(char board[9][9], int& row, int& col){

  // create coordinates
  int top_left[2] = {row-1, col-1};
  int top[2] = {row-1, col};
  int top_right[2] = {row-1, col+1};
  int right[2] = {row, col+1};
  int bot_right[2] = {row+1, col+1};
  int bottom[2] = {row+1, col};
  int bot_left[2] = {row+1, col-1};
  int left[2] = {row, col-1};

  int* around[8] = {top_left, top, top_right, right, bot_right, bottom, bot_left, left};

  // search for mines
  for ( int i = 0; i < 8; i++){
    if(around[i][0]>=0 && around[i][0]<=8 && around[i][1] >= 0 && around[i][1] <= 8){
      if ( board[around[i][0]][around[i][1]] == '?' ){
	row = around[i][0];
	col = around[i][1];
      }
    }
  }
}

// returns true if a risk-free move is available
bool find_safe_move(char revealed[9][9], char* move){

  int index(0); // for assigning to move

  for ( int i = 0; i < 512; i++ ) move[i] = '\0'; // flush move
  
  for ( int row = 0; row < 9; row++){
    for ( int col = 0; col < 9; col++){
      int count_flag(0); // count squares with '*'
      int count_empty(0); // count squares with '?'
      
      if ( isdigit(revealed[row][col]) ){
	count_empty = count_around(revealed, row, col, '?');
	count_flag = count_around(revealed, row, col, '*');

	// continue if no empty cells around
	if ( !count_empty )continue;
	
	int number = static_cast<char>(revealed[row][col]) - '0';
	int r = row;
	int c = col;
       			       
	//cout << row <<col<<" X "<<count_empty << " T " <<  number-count_flag << endl;
	// if number of mines is more than empty cell, flag
	if ( count_empty - (number - count_flag) == 0 ) {
	  find_empty(revealed, r, c);
	  move[index++] =  static_cast<char>(r + 'A');
	  move[index++] =  static_cast<char>(c + '0' + 1);
	  move[index++] = '*';
	  return true;    
	}
	
	// else if all mines are flagged, it is a safe move
	else if ( count_flag == number ){
	  find_empty(revealed, r, c);
	  move[index++] =  static_cast<char>(r + 'A');
	  move[index++] =  static_cast<char>(c + '0' + 1);
	  return true;    
	}
      }
    }
  }
  return false;
}

void solve_board(char mines[9][9], char revealed[9][9], char* moves){
  
  
  
}

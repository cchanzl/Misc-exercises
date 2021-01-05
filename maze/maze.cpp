#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

// returns true if a marker is found on the map and updates the coordinates provided
bool find_marker(char ch, char** maze, const int height, const int width, int& row, int& col){

  for ( int r = 0; r < height; r++){
    for ( int c = 0; c < width; c++){
      if ( maze[r][c] == ch ){
	row = r;
	col = c;
	return true;
      }
    }
  }
  return false;
}

// returns true if movement is valid, and make the move
bool next_step(const char direction, char** maze, int& row, int& col, const int height, const int width){
  // check direction is a valid char
  if ( direction != 'N' && direction != 'S' &&  direction != 'E' && direction != 'W' ){
    return false;
  }

  if ( direction == 'N' ) row--;
  if ( direction == 'S' ) row++;
  if ( direction == 'E' ) col++;
  if ( direction == 'W' ) col--;

  // check within boundary
  if ( row < 0 || row > height-1 || col < 0 || col > width-1 ) return false;

  // check next move is to an empty cell
  if ( maze[row][col] == '#' ||  maze[row][col] == '+' || maze[row][col] == '-' || maze[row][col] == '|') return false;
  
  return true;
}

// returns true if it is a valid solution
bool valid_solution(string path, char** maze, const int height, const int width, int row, int col){
  const char* letter = path.c_str();

  // set starting coordinate for first call of this function
  if ( row == 0 && col == 0 ){
    if ( !find_marker('>', maze, height, width, row, col) ) return false;
  }

  // check if reached exit marker
  if ( maze[row][col] == 'X' ) return true;
  
  // move to next position. return false if next move is invalid
  if ( !next_step(*letter, maze, row, col, height, width) ) return false;

  // call next step
  return valid_solution(letter+1, maze, height, width, row, col);

}

// returns path from start to end if found
string find_path(char** maze, const int height, const int width, const char start, const char end, int row=0, int col=0, char* path = nullptr, int index = 0){

  // set up first run of the function
  if ( row == 0 && col == 0 ){
    if ( !find_marker(start, maze, height, width, row, col) ) return "no solution";
    path = new char[height*width];
  }

  // initilaise parameters used
  int temp_row = row;
  int temp_col = col;
  string answer;
  char direction[4] = {'N', 'S', 'E', 'W'};
  char marker = '#';
  
  for ( int i = 0; i < 4; i++){
    // continue if next step is not valid
    if ( !next_step(direction[i], maze, row, col, height, width) ) {
      row = temp_row;
      col = temp_col;
      continue;
    }
  
    // end case if exit is found
    if ( maze[row][col] == end ) {
      maze[row][col] = marker;
      path[index++] = direction[i];
      path[index] = '\0'; // set null terminating character
      string str(path);
      delete[] path; // to deallocate memory declared on the heap
      return str;
    }

    // mark path
    char temp = maze[row][col];
    maze[row][col] = marker;
    path[index++] = direction[i];

    // recursively call find_path
    answer = find_path(maze, height, width, start, end, row, col, path, index); 
    if ( answer != "no solution" ) return answer;

    // revert previous status if no solution
    maze[row][col] = temp;
    index--;
  }

  // no solution since there is no valid move in all four directions
  return "no solution";

}

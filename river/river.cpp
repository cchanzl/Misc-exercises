#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */
// prints out boat scene. true for left and false for right
char** add_boat(char** scene, bool side, std::string boat){

  // coordinates of boat
  int left_boat[2] = {17, 19}; // row by col
  int right_boat[2] = {17, 36}; // row by col
  
  int row_offset = 6;
  int col_offset_1 = 3;
  int col_offset_2 = 9;

  // get coordinate
  int cor_boat[2] = {left_boat[0], left_boat[1]};
  if ( side == false ) {
    cor_boat[0] = right_boat[0];
    cor_boat[1] = right_boat[1];
  }

  int passenger_1[2] = {cor_boat[0] - row_offset, cor_boat[1] + col_offset_1};
  int passenger_2[2] = {cor_boat[0] - row_offset, cor_boat[1] + col_offset_2};

  // adding passenger
  const char* boat_pgr = boat.c_str();
 
  // add boat
  add_to_scene(scene, cor_boat[0], cor_boat[1], "boat.txt");
  if ( !boat.length() ) return scene; 
  
  // passenger 1
  if ( *boat_pgr == 'M' ) {
    add_to_scene(scene, passenger_1[0], passenger_1[1], "missionary.txt");
  }
  else if ( *boat_pgr == 'C' ){
    add_to_scene(scene, passenger_1[0], passenger_1[1], "cannibal.txt");    
  }

  // passenger 2
  if ( *(boat_pgr+1) == 'M' ) {
    add_to_scene(scene, passenger_2[0], passenger_2[1], "missionary.txt");
  }
  else if ( *(boat_pgr+1) == 'C' ){
    add_to_scene(scene, passenger_2[0], passenger_2[1], "cannibal.txt");    
  }
   
  return scene;
}

// prints out left and right bank. true for left and false for right
char** add_bank(char** scene, bool side, std::string characters){

  // cannot have more than 6 character on a bank
  int length = characters.length();
  if ( length > 7 ) return scene;
  
  // add character on left bank
  int left_bank[2] = {2, 1};
  int right_bank[2] = {left_bank[0], 54};
  
  int person_offset = 6;
  int cannibal_offset_row = 9;

  // get coordinates
  int cor_char[2] = {left_bank[0], left_bank[1]};
  if ( side == false ) {
    cor_char[0] = right_bank[0];
    cor_char[1] = right_bank[1];
  }

  // add characters
  const char* character = characters.c_str(); 
  int m_count(0);
  int c_count(0);
  
  for ( int i = 0; i < length; i++){
    if ( *(character+i) == 'M' ){
      add_to_scene(scene, cor_char[0], cor_char[1] + m_count*person_offset, "missionary.txt");
      m_count++;
    }
    else if ( *(character+i) == 'C' ){
      add_to_scene(scene, cor_char[0] + cannibal_offset_row, cor_char[1] + c_count*person_offset, "cannibal.txt");
      c_count++; 
    }
  }
  
  return scene;

}

int count_char(string characters, char type){
  int count(0);
  int length = characters.length();
  const char* char_ptr = characters.c_str(); 
   
  for ( int i = 0; i < length; i++ ){
    if ( *(char_ptr+i) == type ) count++;
  }

  return count;
}

string make_right(string left, string boat){
  int c_count(0);
  int m_count(0);

  m_count = count_char(left, 'M') + count_char(boat, 'M');
  c_count = count_char(left, 'C') + count_char(boat, 'C');
  
  string right;
  for ( int i = m_count; i < 3; i++ ) right.push_back('M');
  for ( int i = c_count; i < 3; i++ ) right.push_back('C');
  //  cout << right << endl;
   
  return right;
}

char** make_river_scene(std::string left, std::string boat){

  // allocate memory
  char **scene = create_scene();
  
  // add base scenes
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");

  // find boat position
  bool boat_pos = false; // right
  if ( count_char(left, 'B' )== 1 ) boat_pos = true; // left 
  
  // add boat
  add_boat(scene, boat_pos, boat);

  // add bank characters
  string right = make_right(left, boat);
  add_bank(scene, true, left);
  add_bank(scene, false, right);
  
  return scene;
}

// check if boat arraiving at bank will result in missionary being eaten
bool is_eaten(string bank, string boat){

  int total_m = count_char(bank, 'M') + count_char(boat, 'M');
  int total_c = count_char(bank, 'C') + count_char(boat, 'C');
  if ( total_c > total_m && total_m > 0 ) return true;
  return false;
}

// update position for all 3 places
void load_boat(string& bank, const char* targets){
  int count_c, count_m;
  int boat_count_c, boat_count_m;
  int final_count_c, final_count_m;
  int total_count;
  char new_bank[bank.length()];
  
  // loading 
  count_m = count_char(bank, 'M');
  count_c = count_char(bank, 'C');
  boat_count_m = count_char(targets, 'M');
  boat_count_c = count_char(targets, 'C');
  
  final_count_m = count_m - boat_count_m;
  final_count_c = count_c - boat_count_c;
  total_count = final_count_m + final_count_c;
  
  for ( int i = 0; i < final_count_m; i++) new_bank[i] = 'M';
  for ( int i = final_count_m; i < total_count; i++) new_bank[i] = 'C';

  new_bank[total_count] = 'B';
  bank = new_bank;
}

int perform_crossing(char* left2, std::string targets){
  
  const char* target = targets.c_str();
  string left(left2);
  
  // allocate memory
  char**  scene = create_scene();
  scene = make_river_scene("BCCCMMM","");
  //print_scene(scene);
  
  // check that target is correct
  if ( count_char(targets, 'M') > count_char(targets, 'M') ) return -3;
  if ( count_char(targets, 'C') > count_char(targets, 'C') ) return -3;
  
  // find boat position
  bool boat_pos = false; // right
  if ( count_char(left, 'B') == 1 ) boat_pos = true; // left 

  // make right bank
  string right = make_right(left, targets);
  
  // check if boat is empty
  if ( !targets.length() ) return -3;

  // load to boat
  cout << "Loading the boat..." << endl;
  if ( boat_pos ) load_boat(left, target);
  else load_boat(right, target);
  scene = make_river_scene(left, targets);
  print_scene(scene);
  cout << endl;
  destroy_scene(scene);
 
  // crossing the river
  cout << "Crossing the river..." << endl;
  if ( boat_pos ) {
    left.pop_back();
    right.push_back('B'); // arrived at right bank
  }
  else {
    right.pop_back(); 
    left.push_back('B'); // arrived at left bank
  }
  scene = make_river_scene(left, targets);
  print_scene(scene);
  cout << endl;
  destroy_scene(scene); 

  // arriving on opposite bank
  cout << "Unloading the boat..." << endl;
  if ( boat_pos ) {
    right = right + targets;
  }
  else {
    left = left + targets;
  }
  //cerr << left << "123" << endl;
  scene = make_river_scene(left, "");
  print_scene(scene);
  destroy_scene(scene); 

  // update left2
  int i(0);
  while ( left[i] != '\0'){
    *left2 = left[i];
    left2++;
    i++;
  }
  *left2 = '\0';
  
  // check if missionaries are eaten on either end
  right = make_right(left, "");
  if ( is_eaten(right, "") ) return -4;
  if ( is_eaten(left, "") ) return -4;

  // return
  return 1;
}

int play_game(){
  char left[10] = "CCCMMMB";
  int count (0);
  string boat;
  
  while (true) {
    
    if ( count == 0 ) {
      cout << "Please enter passengers (1 or 2 upper case char M or C) to load onto the boat." << endl;
    }

    cout << "Please enter next move" << endl;
    
    cin >> boat;
    int result =  perform_crossing(left, boat);
    cerr << left << endl;
    std::string right = make_right(left,"");
    if ( result < 0 ) return result;
    if ( count_char(right, 'M') == 3 && count_char(right, 'C') == 3 ) return 2;
    count++;
  }

}

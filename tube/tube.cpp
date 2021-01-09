#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
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

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

// updates r and c based on where target is found
bool get_symbol_position(char** map, const int height, const int width, const char target, int& r, int& c){
  
  // check input
  if ( height <=  0 || width <= 0 ) {
    r = -1;
    c = -1;
    return false;
    }

  // find target
  for ( int h = 0; h < height; h++){
    for (  int w = 0; w < width; w++){
      //cout << h << w << " " << map[h][w] << endl;
      if ( map[h][w] == target ) {
	r = h;
	c = w;
	return true;
      }      
    }
  }
  return false;  
}

// return symbol for station or line
char get_symbol_for_station_or_line(std::string name){

  // find symbol for station
  std::ifstream in("stations.txt");
  if (!in) {
    std::cerr << "Error: Unable to open file" << std::endl;
    return ' ';
  }

  string line;
  while(getline(in, line)){
    std::size_t found = line.find(name);
    if (found != std::string::npos){ 
      in.close();
      return line[0];
    }
  }

  in.close();

  // find symbol for line
  std::ifstream in2("lines.txt");
  if (!in2) {
    std::cerr << "Error: Unable to open file" << std::endl;
    return ' ';
  }

  string line2;
  while(getline(in2, line2)){
    std::size_t found = line2.find(name);
    if (found != std::string::npos){ 
      in2.close();
      return line2[0];
    }
  }

  in2.close();

  // return space if not found in both file
  return ' ';
  
}

// returns the number of line changes and updates the destination with the final station
int validate_route(char** map, const int height, const int width,
		   std::string start_station, std::string route, char* destination){

  // flush destination
  for ( int i = 0; i < 512; i++ ) destination[i] = '\0';
  
  // start_station must be a valid station
  char start = get_symbol_for_station_or_line(start_station);
  //cout << start << endl;
  if ( start == ' ' ) {
    //cout << "test" << endl;
    return -1;
  }
  // find start station coordinates
  int r, c;
  char result =  get_symbol_position(map, height, width, start, r, c);
  if ( result == ' ' ) {
    //cout <<r <<  " test " << c << endl;
    return -1;
  }
  // initialise count for line change
  int count_line_change(0);
  char prev_line, current_line;
  int reach_station(0);
  int count(0);

  // create vector to store visited stations
  vector<int> visited;
  int position = r*width + c;
  visited.push_back(position);
  //cout << position << " Original" << endl;
  // check input parameter route
  const char* direction = route.c_str();
  while ( *direction != '\0' ){
    if ( *direction == ',' ) {
      direction++;
      continue;
    }

    // find next direction
    string direct;
    while ( *direction != ',' && *direction != '\0'){
      direct.push_back(*direction);
      direction++;
    }
    
    const char * dir = direct.c_str();
    //cout <<  string_to_direction(dir) << endl;
    if ( string_to_direction(dir) == 8 ) return -5;

    // make move
    switch( string_to_direction(dir) ){
    case 0:
      r -= 1;
      break;

    case 1:
      r += 1;
      break;

    case 2:
      c -= 1;
      break;

    case 3:
      c += 1;
      break;

    case 4:
      r -= 1;
      c += 1;
      break;

    case 5:
      c -= 1;
      r -= 1;
      break;

    case 6:
      c += 1;
      r += 1;
      break;

    case 7:
      c -= 1;
      r += 1;
      break;

    default:
      return -5; 
      break;
    }
 
    // check out of bounds condition
    if ( r < 0 || r > height - 1 || c < 0 || c > width - 1 ) return -7;

    // check if off track
    if ( map[r][c] == ' ' ) return -6;
    //cout << map [r][c] << endl;

    // add coordination to visited vector
    int position = r*width + c;
    //cout << position << "T" << visited.size() << endl;
    visited.push_back(position); // add to visited
    int size = visited.size();
    //cout << visited[size-1] << "TEST" <<endl;
    if ( visited[size-1] == visited[size-3] ) return -4;
    
   
    // check if reach a station
    if ( isalnum(map[r][c]) ) reach_station = 1;

    // count if line change occurred
    if ( count++ == 0 ) current_line = map[r][c];
    prev_line = current_line;
    if ( !isalnum(map[r][c]) ) current_line = map[r][c]; // check for line change
    if ( reach_station == 2 && prev_line != current_line ) { // changed line
      count_line_change++;
      reach_station = 0; // reset indicator
    }
    else if ( prev_line != current_line ){ // change line without reaching a station
      return -3;
    }
    
    // reset reach_station if line change did not happen
    if ( reach_station >  0 ) reach_station++;
    if ( reach_station == 3 ) reach_station = 0; // reach station but did not change
  }

  // exit while loop with end point
  if ( !isalnum(map[r][c]) ) return -2; // end point not a station

  // update destination
  int x, y;
  char final_stop;
  for ( char i = '0'; i < '9'; i++){
    x = 0;
    y = 0;
    get_symbol_position(map, height, width, i, x, y);
    if ( x == r && y == c ) final_stop = i; 
  }

  for ( char i = 'A'; i < 'Z'; i++){
    x = 0;
    y = 0;
    get_symbol_position(map, height, width, i, x, y);
    if ( x == r && y == c ) final_stop = i; 
  }

  for ( char i = 'a'; i < 'z'; i++){
    x = 0;
    y = 0;
    get_symbol_position(map, height, width, i, x, y);
    if ( x == r && y == c ) final_stop = i; 
  }
  
  // get station name  
  std::ifstream in("stations.txt");
  if (!in) {
    std::cerr << "Error: Unable to open file" << std::endl;
    return ' ';
  }
  //cout << final_stop << endl;
  string line;
  while(getline(in, line)){
    //cout << line[0] << final_stop << endl;
    if (line[0] == final_stop){
      int i(2); // skip first two char
      while(line[i] != '\0'){
	destination[i-2] = line[i];
	i++;
      }
      break;
    }
  }

  in.close();
  
  return count_line_change;
  
}

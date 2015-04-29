/* 
 * File:   main.cpp
 * Author: btacklind
 *
 * Created on April 12, 2015, 9:37 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <dirent.h>

#include "ProblemHandler.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  
  if (argc != 2){
    cout << argc << " Not right number of argument(s)" << endl;
    exit(-1);
  }
  
  struct answers list [200];
  int numAnswers = 0;
  
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (argv[1])) != NULL) {
  /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      if (string(ent->d_name).find(".tsp") != -1){
        
        string str = string(argv[1]).append("\\\\").append(ent->d_name);
        
        //cout << ent->d_name << endl;
        list[numAnswers++] = ProblemHandler::readFile(str.c_str());
        
      }
    }
  closedir (dir);
  } else {
  /* could not open directory */
    cout << "failed to open directory" << endl;
    exit(-1);
  }
  
  ifstream opt("OptimalTours.txt");
  
  if(!opt.is_open()){
    cout << "failed to open file" << endl;
    return EXIT_FAILURE;
  }
  
  cout << "Printing Answers" << endl;
  for(int j = 0 ; j < numAnswers; j++){
    if (!list[j].empty){
      cout << list[j].name << ", " << list[j].HCP << ", " << list[j].shortPath;
      
      string l;
      int temp;
      while(getline(opt, l) && list[j].name.find(l.substr(0, (temp=l.find(":")) - 1)) == -1 );
      cout << ", " << l.substr(temp+2) << endl;
      
    }
  }
  
  opt.close();
  
  return EXIT_SUCCESS;
}




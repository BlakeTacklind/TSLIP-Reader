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
  
  ProblemHandler problems [200];
  ProblemHandler p;
  int i = 0;
  
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (argv[1])) != NULL) {
  /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      if (string(ent->d_name).find(".tsp") != -1){
        //problems[i++] = ProblemHandler::readFile((string(argv[1]).append(ent->d_name)).c_str());
        string str = string(argv[1]).append("\\\\").append(ent->d_name);
        cout << str << endl;
        ProblemHandler::readFile(str.c_str(), &p);
        //cout << "works?" << endl;
      }
    }
  closedir (dir);
  } else {
  /* could not open directory */
    cout << "failed to open directory" << endl;
    exit(-1);
  }
  //Check file and read to get general data
  /*
  */
  
  //switch to which form
  /*
  switch (p.getProblemType()){
    case TSP: 
      cout << "This is a TSP problem" << endl;
      break;
    case HCP:
      cout << "This is a HCP problem" << endl;
      cout << "The Hamiltonian cycle distance of " << p.getName() << " is: " << p.solveHCP() << endl;
      break;
    case Other:
    default:
      cout << "Unsupported Problem to Solve" << endl;
  }
  */
  
  //cout << "The Hamiltonian cycle distance of " << p.getName() << " is: " << p.solveHCP() << endl;
  
  //return EXIT_SUCCESS;
}




/* 
 * File:   main.cpp
 * Author: btacklind
 *
 * Created on April 12, 2015, 9:37 PM
 */

#include <cstdlib>
#include <stdio.h>

#include "ProblemHandler.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  ProblemHandler p;
  
  //Check file and read to get general data
  if (argc == 2)
    p = ProblemHandler::readFile(argv[1]);
  else{
    printf("Not right number of argument(s)");
    exit(-1);
  }
  
  //switch to which form
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
      printf("Unsupported Problem to Solve");
  }
  
  
  cout << "The Hamiltonian cycle distance of " << p.getName() << " is: " << p.solveHCP() << endl;
  
  return 0;
}




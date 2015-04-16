/* 
 * File:   ProblemHandler.h
 * Author: btacklind
 *
 * Created on April 12, 2015, 9:54 PM
 */

#ifndef PROBLEMHANDLER_H
#define	PROBLEMHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include "Node.h"
using namespace std;

enum Problem{
  Other, TSP, HCP
}; 

enum Edge_Type{
    ET_NONE, EXPLICIT, EUC_2D, EUC_3D, MAX_2D, MAX_3D, MAN_2D, MAN_3D, CEIL_2D, 
    GEO, ATT, XRAY1, XRAY2, SPECIAL
};

enum Edge_Weight_Format{
    EWF_NONE, FUNCTION, FULL_MATRIX, UPPER_ROW, LOWER_ROW, UPPER_DIAG_ROW, 
    LOWER_DIAG_ROW, UPPER_COL, LOWER_COL, UPPER_DIAG_COL, LOWER_DIAG_COL
};

enum EDGE_DATA_FORMAT{
    EDF_NONE, EDGE_LIST, ADJ_LIST
};

class ProblemHandler {
public:
    static ProblemHandler readFile(char* input);
    
    ProblemHandler();
    ProblemHandler(const ProblemHandler& orig);
    virtual ~ProblemHandler();
    
    inline string getName(){return name;};
    inline Problem getProblemType(){return ptype;};
    
    float getDistance(int node1, int node2);
    
    float solveHCP();
    
private:
    string name;
    Problem ptype;
    int dimension;
    Edge_Type etype;
    Edge_Weight_Format ewtype;
    EDGE_DATA_FORMAT edtype;
    NODE_COORD_TYPE ctype;
    Node* nodes;
    
    static string searchForSpecificLine (ifstream* f, string line);
    static void getNodeData(string line, ProblemHandler* p);
};

#endif	/* PROBLEMHANDLER_H */


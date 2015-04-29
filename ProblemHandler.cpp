/* 
 * File:   ProblemHandler.cpp
 * Author: btacklind
 * 
 * Created on April 12, 2015, 9:54 PM
 */

#include "ProblemHandler.h"
#include <math.h>
#include <float.h>
using namespace std;

ProblemHandler::ProblemHandler() {
  ptype = NONE;
  dimension = 0;
  etype = ET_NONE;
  ewtype = EWF_NONE;
  edtype = EDF_NONE;
  ctype = C_NONE;
}

ProblemHandler::ProblemHandler(const ProblemHandler& orig) {
}

ProblemHandler::~ProblemHandler() {
  if (nodes != NULL) free(nodes);
}

/*
 * This function reads the input file to get its data
 */
struct answers ProblemHandler::readFile(const char* input) {
  ifstream  file(input);
  
  //In case the file fails to open spits out error and ends program
  if (!file.is_open()){
    printf("failed to open file");
    exit(-1);
  }
  
  //create a new problem
  ProblemHandler p = ProblemHandler();
  
  getLineData(&file, &p);
  
  //file assumed fully read, returns object created
  file.close();
  
  if (p.etype != EUC_2D && p.etype != GEO)
    return empty(p.name);
  
  if (p.name.find("burma14") != -1)
    p.distanceList();
  
  struct answers a;
  a.empty = false;
  a.HCP = p.solveHCP();
  a.shortPath = p.solveNearestNeighborCycle();
  a.name = p.getName();
  
  //cout << a.HCP << ", " << a.shortPath << endl;
  
  return a;
}

answers ProblemHandler::empty(string name) {
  struct answers a;
  a.empty = true;
  a.HCP = 0;
  a.shortPath = 0;
  a.name = name;
  return a;
}

void ProblemHandler::getLineData(ifstream* f, ProblemHandler* p) {
  string l;
  string out;
  while(getline(*f, l)){
    
    //cout << l << endl;
    
    if( !(out = getStringFromLine(l, "NAME: ")).empty() || !(out = getStringFromLine(l, "NAME : ")).empty() ){
      p->name = out;
      //cout << "name: " << p->name << endl;
    }
    else if( !(out = getStringFromLine(l, "DIMENSION: ")).empty() || !(out = getStringFromLine(l, "DIMENSION : ")).empty()){
      p->dimension = atoi(out.c_str());
      //cout << "dim: " << p->dimension << " size " << p->dimension * sizeof(Node) << endl;
      
      p->nodes = (Node*)malloc(p->dimension * sizeof(Node) + sizeof(Node));
      if(p->nodes == NULL){
        cout << "failed to malloc!" << endl;
        exit(1);
      }
    }
    else if( !(out = getStringFromLine(l, "EDGE_WEIGHT_TYPE: ")).empty() || !(out = getStringFromLine(l, "EDGE_WEIGHT_TYPE : ")).empty()){
      if(out.find("EXPLICIT") != -1){
        p->etype = EXPLICIT;
        p->ctype = NO_COORDS;
      }
      else if(out.find("EUC_2D") != -1){
        p->etype = EUC_2D;
        p->ctype = TWOD_COORDS;
      }
      else if(out.find("EUC_3D") != -1){
        p->etype = EUC_3D;
        p->ctype = THREED_COORDS;
      }
      else if(out.find("MAX_2D") != -1){
        p->etype = MAX_2D;
        p->ctype = TWOD_COORDS;
      }
      else if(out.find("MAX_3D") != -1){
        p->etype = MAX_3D;
        p->ctype = THREED_COORDS;
      }
      else if(out.find("MAN_2D") != -1){
        p->etype = MAN_2D;
        p->ctype = TWOD_COORDS;
      }
      else if(out.find("MAN_3D") != -1){
        p->etype = MAN_3D;
        p->ctype = THREED_COORDS;
      }
      else if(out.find("CEIL_2D") != -1){ 
        p->etype = CEIL_2D;
        p->ctype = TWOD_COORDS;
      }
      else if(out.find("GEO") != -1){
        p->etype = GEO;
        p->ctype = TWOD_COORDS;
      }
      else if(out.find("ATT") != -1){
        p->etype = ATT;
        p->ctype = TWOD_COORDS;
      }
      else if(out.find("XRAY1") != -1)
        p->etype = XRAY1;
      else if(out.find("XRAY2") != -1)
        p->etype = XRAY2;
      else if(out.find("SPECIAL") != -1)
        p->etype = SPECIAL;
      else{
        cout << "failed to get edge type" << endl;
        exit(-1);
      }
      
      //cout << "Edge Type: " << p->etype << endl;
      //cout << "Coord Type: " << p->ctype << endl;
    }
    else if( l.find("NODE_COORD_SECTION") != -1){
      if (p->dimension == 0){
        cout << "node number not set!" << endl;
        exit(-1);
      }
      
      //cout << "Dimensions " << p->dimension << endl;
      
      //cout << "reading node data..." << endl;
      for (int i = 0; i < p->dimension; i++){
        if (!getline(*f, out)){
          cout << "End of file reached in coordinates" << endl;
          exit(-1);
        }
        
        //cout << "On node " << i << " ";
        
        getNodeData(out, p);
      }
      //cout<< "done nodes" << endl;
    }
    else if( !(out = getStringFromLine(l, "TYPE: ")).empty() && (out = getStringFromLine(l, "_TYPE: ")).empty() ){
      if (out.find("TSP") != -1)
        p->ptype = TSP;
      else if (out.find("HCP") != -1)
        p->ptype = HCP;
      else
        p->ptype = Other;
      
      //cout << "Problem Type: " << p->ptype << endl;
    }
    
    if ( l.find("EOF") != -1){
      
      //cout << "End of File reached normally" << endl;
      return;
    }
  }
  
  //cout << "Minor Error: End of file reached before 'EOF' tag" << endl;
  return;
}

string ProblemHandler::getStringFromLine(string line, string search) {
  int pos;
  if ( (pos = line.find(search)) == -1 )
    return string("");
  
  return line.substr(pos + search.length());
}

/*
 * This function searches from a point in a file to find the next occurrence 
 * at the beginning of the line of the given string.
 */
string ProblemHandler::searchForSpecificLine(ifstream* f, string line) {
  
  string l;
  int pos;
  while(1){
    if (!getline(*f,l)){
      cout << "End of file reached before \"" << line << "\" found" << endl;
      f->close();
      exit(-1);
    }
    
    if((pos = l.find(line)) != -1){
      return l.substr(pos + line.length());
    }
    
  }
  
  return NULL;
}

/*
 * from a line of input data file, gets the node and coordinates of the node
 * defined on that line
 */
void ProblemHandler::getNodeData(string line, ProblemHandler* p) {
  
  //parses line into <space> delimited pieces
  int pos;
  string token;
  list<string> tokens;
  while ((pos = line.find(" ")) != -1) {
    token = line.substr(0, pos);
    if (token != "") tokens.push_back(token);
    line.erase(0, pos + 1);
  }
  if (line != "") tokens.push_back(line);
  
  if (p->ctype == TWOD_COORDS){
    Node n = Node(atoi(tokens.front().c_str()));

    tokens.pop_front();
    n.setX(atof(tokens.front().c_str()));
    tokens.pop_front();
    n.setY(atof(tokens.front().c_str()));

    if(n.getNumber() > p->dimension || n.getNumber() < 1){
      cout << "Given node number out of range" <<endl;
      exit(-1);
    }
    p->nodes[n.getNumber()] = n;
    //cout << "got node " << n.getNumber() << " X: " << n.x() << endl;
  }
  else if(p->ctype == THREED_COORDS){
    Node n = Node(atoi(tokens.front().c_str()));

    tokens.pop_front();
    n.setX(atof(tokens.front().c_str()));
    tokens.pop_front();
    n.setY(atof(tokens.front().c_str()));
    tokens.pop_front();
    n.setZ(atof(tokens.front().c_str()));

    if(n.getNumber() > p->dimension || n.getNumber() < 1){
      cout << "Given node number out of range" <<endl;
      exit(-1);
    }
    p->nodes[n.getNumber()] = n;
    //cout << "got node " << n.getNumber() << endl;
  }
  else if(p->ctype == NO_COORDS){
    Node n = Node(atoi(tokens.front().c_str()));

    if(n.getNumber() > p->dimension || n.getNumber() < 1){
      cout << "Given node number out of range" <<endl;
      exit(-1);
    }
    p->nodes[n.getNumber()] = n;
    //cout << "got node " << n.getNumber() << endl;
  }
  else{
    cout << "No coordinate type given!" << endl;
    exit(-1);
  }
  
}


//Constants for getDistance
static const float RRR = 6378.388;
static const float PI = 3.1415926;
/*
 * Gets the distances base on the distance measured type and the nodes of 
 * two nodes referenced in the problem.
 */
float ProblemHandler::getDistance(int node1, int node2) {
  if (node1 < 1 || node1 > dimension || node2 < 1 || node2 > dimension){
    cout << "node not in range " << node1 << " " << node2 << endl;
    return 0;
  }
  
  if (etype == ET_NONE){
    cout << "edge type not set" << endl;
    return 0;
  }
  
  switch(etype){
    case GEO:
      int deg = int(nodes[node1].x());
      float min = nodes[node1].x() - deg;
      float lat1 = PI * (deg + 5 * min / 3) / 180;
      deg = int(nodes[node1].y());
      min = nodes[node1].y() - deg;
      float lng1 = PI * (deg + 5 * min / 3) / 180;
      deg = int(nodes[node2].x());
      min = nodes[node2].x() - deg;
      float lat2 = PI * (deg + 5 * min / 3) / 180;
      deg = int(nodes[node2].y());
      min = nodes[node2].y() - deg;
      float lng2 = PI * (deg + 5 * min / 3) / 180;
      float q1 = cos( lng1 - lng2 );
      float q2 = cos( lat1 - lat2 );
      float q3 = cos( lat1 + lat2 );
      return ( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
    case EUC_2D:
      return sqrt( pow((nodes[node1].x() - nodes[node2].x()),2) + pow((nodes[node1].y() - nodes[node2].y()),2) );
    default:
      cout << "Distance measurement not implemented" << endl;
      return 0;
  }
}

/*
 * calculates the Hamiltonian cycle distance
 * sum of the distances from nodes 1->2, 2->3, ... , (n-1)->n, n->1
 */
float ProblemHandler::solveHCP() {
  float dist = getDistance(1, dimension);
  
  for (int i = 1; i < dimension; i++){
    dist += getDistance(i, i+1);
  }
  
  return dist;
}

float ProblemHandler::solveNearestNeighborCycle() {
  float dist = 0;
  bool used [dimension + 1];
  
  for (int i = 0; i <= dimension ; i++)
    used[i] = false;
  
  used[1] = true;
  int onNode = 1;
  
  //cout << name << " nearest neighbor tour: 1 ";
  
  for(int i = 1; i < dimension; i++){
    float smallest = FLT_MAX;
    int NxtNode = 0;
    for(int j = 1; j <= dimension; j++){
      float temp;
      if(!used[j]){
        if(NxtNode == 0 || (temp = getDistance(onNode, j)) < smallest){
          smallest = temp;
          NxtNode = j;
        }
      }
    }
    
    //cout << NxtNode << " ";
    used[NxtNode] = true;
    onNode = NxtNode;
    dist += smallest;
  }
  
  dist += getDistance(1, onNode);
  //cout << "1" << endl;
  
  return dist;
}

void ProblemHandler::distanceList() {
  ofstream out;
  string fileName = name;
  
  out.open(fileName.append(".txt").c_str());
  
  out << dimension << endl;
  
  for(int i = 1; i <= dimension; i++){
    for(int j = i+1; j <= dimension; j++){
      out << i << " " << j << " " << getDistance(i, j) << endl;
    }
  }
  
  out.close();
}

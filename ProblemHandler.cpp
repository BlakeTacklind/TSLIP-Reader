/* 
 * File:   ProblemHandler.cpp
 * Author: btacklind
 * 
 * Created on April 12, 2015, 9:54 PM
 */

#include "ProblemHandler.h"
#include <math.h>
using namespace std;

ProblemHandler::ProblemHandler() {
}

ProblemHandler::ProblemHandler(const ProblemHandler& orig) {
}

ProblemHandler::~ProblemHandler() {
  if (nodes != NULL) free(nodes);
}

ProblemHandler ProblemHandler::readFile(char* input) {
  ifstream  file(input);
  if (!file.is_open()){
    printf("failed to open file");
    exit(-1);
  }
  
  ProblemHandler p = ProblemHandler();
  
  p.name = searchForSpecificLine(&file, string("NAME: "));
  
  string line = searchForSpecificLine(&file, string("TYPE: "));
  if (line.find("TSP") != -1)
    p.ptype = TSP;
  else if (line.find("HCP") != -1)
    p.ptype = HCP;
  else
    p.ptype = Other;
  
  p.dimension = atoi(searchForSpecificLine(&file, string("DIMENSION: ")).c_str());
  p.nodes = (Node*)malloc(p.dimension * sizeof(Node));
  if(p.nodes == NULL){
    cout << "failed to malloc!" << endl;
    exit(-1);
  }
  
  line = searchForSpecificLine(&file, string("EDGE_WEIGHT_TYPE: "));
  if(line.find("EXPLICIT") != -1){
    p.etype = EXPLICIT;
    p.ctype = NO_COORDS;
  }
  else if(line.find("EUC_2D") != -1){
    p.etype = EUC_2D;
    p.ctype = TWOD_COORDS;
  }
  else if(line.find("EUC_3D") != -1){
    p.etype = EUC_3D;
    p.ctype = THREED_COORDS;
  }
  else if(line.find("MAX_2D") != -1){
    p.etype = MAX_2D;
    p.ctype = TWOD_COORDS;
  }
  else if(line.find("MAX_3D") != -1){
    p.etype = MAX_3D;
    p.ctype = THREED_COORDS;
  }
  else if(line.find("MAN_2D") != -1){
    p.etype = MAN_2D;
    p.ctype = TWOD_COORDS;
  }
  else if(line.find("MAN_3D") != -1){
    p.etype = MAN_3D;
    p.ctype = THREED_COORDS;
  }
  else if(line.find("CEIL_2D") != -1){ 
    p.etype = CEIL_2D;
    p.ctype = TWOD_COORDS;
  }
  else if(line.find("GEO") != -1){
    p.etype = GEO;
    p.ctype = TWOD_COORDS;
  }
  else if(line.find("ATT") != -1)
    p.etype = ATT;
  else if(line.find("XRAY1") != -1)
    p.etype = XRAY1;
  else if(line.find("XRAY2") != -1)
    p.etype = XRAY2;
  else if(line.find("SPECIAL") != -1)
    p.etype = SPECIAL;
  else{
    cout << "failed to get edge type" << endl;
    exit(-1);
  }
  
  searchForSpecificLine(&file, string("NODE_COORD_SECTION"));
  for (int i = 0; i < p.dimension; i++){
    getline(file, line);
    getNodeData(line, &p);
  }
  
  file.close();
  return p;
}

string ProblemHandler::searchForSpecificLine(ifstream* f, string line) {
  
  string l;
  int pos;
  while(1){
    if (!getline(*f,l)){
      cout << "End of file reached before \"" << line << "\" found" << endl;
      f->close();
      exit(-1);
    }
    
    //cout << l << endl;
    
    if((pos = l.find(line)) != -1){
      return l.substr(pos + line.length());
    }
    
  }
  
  return NULL;
}

void ProblemHandler::getNodeData(string line, ProblemHandler* p) {
  int pos;
  string token;
  list<string> tokens;
  while ((pos = line.find(" ")) != -1) {
    token = line.substr(0, pos);
    if (token != "") tokens.push_back(token);
    line.erase(0, pos + 1);
  }
  if (line != "") tokens.push_back(line);
  
  float coords[3];
  
  if (p->ctype == TWOD_COORDS){
    Node n = Node(atoi(tokens.front().c_str()), p->ctype);

    tokens.pop_front();
    coords[0] = atof(tokens.front().c_str());
    tokens.pop_front();
    coords[1] = atof(tokens.front().c_str());
    n.setCoords(coords);

    if(n.getNumber() > p->dimension || n.getNumber() < 1){
      cout << "Given node number out of range" <<endl;
      exit(-1);
    }
    p->nodes[n.getNumber()] = n;
  }
  else if(p->ctype == THREED_COORDS){
    Node n = Node(atoi(tokens.front().c_str()), p->ctype);

    tokens.pop_front();
    coords[0] = atof(tokens.front().c_str());
    tokens.pop_front();
    coords[1] = atof(tokens.front().c_str());
    tokens.pop_front();
    coords[2] = atof(tokens.front().c_str());
    n.setCoords(coords);

    if(n.getNumber() > p->dimension || n.getNumber() < 1){
      cout << "Given node number out of range" <<endl;
      exit(-1);
    }
    p->nodes[n.getNumber()] = n;
  }
  else if(p->ctype == NO_COORDS){
    Node n = Node(atoi(tokens.front().c_str()), p->ctype);

    if(n.getNumber() > p->dimension || n.getNumber() < 1){
      cout << "Given node number out of range" <<endl;
      exit(-1);
    }
    p->nodes[n.getNumber()] = n;
  }
  else{
    cout << "No coordinate type given!" << endl;
    exit(-1);
  }
}

static const float RRR = 6378.388;
static const float PI = 3.1415926;

float ProblemHandler::getDistance(int node1, int node2) {
  if (node1 < 1 || node1 > dimension || node2 < 1 || node2 > dimension){
    cout << "node not in range" << endl;
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
    default:
      cout << "Distance measurement not implemented" << endl;
      return 0;
  }
}

float ProblemHandler::solveHCP() {
  float dist = getDistance(1, dimension);
  
  for (int i = 1; i < dimension; i++){
    dist += getDistance(i, i+1);
  }
  
  return dist;
}

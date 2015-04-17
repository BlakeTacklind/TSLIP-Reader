/* 
 * File:   Node.cpp
 * Author: btacklind
 * 
 * Created on April 12, 2015, 10:15 PM
 */

#include "Node.h"
#include "ProblemHandler.h"

Node::Node(int number, NODE_COORD_TYPE type) {
  num = number;
  ctype = type;
}

Node::Node(const Node& orig) {
}

Node::~Node() {
}

/*
 * Sets the coordinates of this node.
 */
void Node::setCoords(float* in) {
  if (ctype == TWOD_COORDS){
    coords[0] = in[0];
    coords[1] = in[1];
  }
  else if (ctype == THREED_COORDS){
    coords[0] = in[0];
    coords[1] = in[1];
    coords[2] = in[2];
  }
  else if (ctype == NO_COORDS){
    cout << "No coordinates needed" << endl;
  }
}

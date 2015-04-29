/* 
 * File:   Node.h
 * Author: btacklind
 *
 * Created on April 12, 2015, 10:15 PM
 */

#ifndef NODE_H
#define	NODE_H

#include<list>
using namespace std;

enum NODE_COORD_TYPE{
    C_NONE, TWOD_COORDS, THREED_COORDS, NO_COORDS
};

/*
 * container for the data of a node.
 */
class Node {
public:
    Node(int number);
    Node(const Node& orig);
    virtual ~Node();
    
    inline float x(){return coords[0];};
    inline float y(){return coords[1];};
    inline float z(){return coords[2];};
    
    inline void setX(float x){coords[0] = x;};
    inline void setY(float y){coords[1] = y;};
    inline void setZ(float z){coords[2] = z;};
    inline int getNumber(){return num;};
private:
    int num;
    float coords[3];
    //list<Node> Adjacent;
};

#endif	/* NODE_H */


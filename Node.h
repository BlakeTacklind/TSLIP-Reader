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
    TWOD_COORDS, THREED_COORDS, NO_COORDS
};

class Node {
public:
    Node(int number, NODE_COORD_TYPE type);
    Node(const Node& orig);
    virtual ~Node();
    
    inline float x(){return coords[0];};
    inline float y(){return coords[1];};
    inline float z(){return coords[2];};
    
    void setCoords(float* in);
    inline int getNumber(){return num;};
private:
    int num;
    float coords[3];
    NODE_COORD_TYPE ctype;
    //list<Node> Adjacent;
};

#endif	/* NODE_H */


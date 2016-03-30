#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>

namespace robot
{

#ifndef MAX_ANGLE
#define MAX_ANGLE 180
#endif

struct Point
{
    float x;
    float y;
    float z;
};

struct Velocity
{
    double x;
    double y;
    double z;
};

typedef struct
{
  
    Point pos;
    int id;
    float time;
    std::string comment;
    
} SafeSpot;

}
#endif                                                      // STRUCTURES_HPP

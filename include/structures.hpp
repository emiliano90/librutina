#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>

struct Point {
	float x;
	float y;
};

struct Point3D {
	float x;
	float y;
	float z;
};

struct Velocity {
	double x;
	double y;
	double z;
};

typedef struct {

	Point pos;
	int id;
	float time;
	std::string comment;

} SafeSpot;

#endif                                                      // STRUCTURES_HPP

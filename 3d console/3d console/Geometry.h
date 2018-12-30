#pragma once

#include <vector>
#include <list>

struct vector3f {
	float x, y, z;
};
struct triangle{
	vector3f vertices[3];
};

struct mesh {
	std::vector<triangle> triangles;
};

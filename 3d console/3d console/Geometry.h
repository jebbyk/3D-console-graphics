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

struct matrix4x4 {
	float matrix[4][4] = { 0 };
};

void MultMatrixVector(vector3f &i, vector3f &o, matrix4x4 &m)
{
	o.x = i.x*m.matrix[0][0] + i.y*m.matrix[1][0] + i.z*m.matrix[2][0] + m.matrix[3][0];
	o.y = i.x*m.matrix[0][1] + i.y*m.matrix[1][1] + i.z*m.matrix[2][1] + m.matrix[3][1];
	o.z = i.x*m.matrix[0][2] + i.y*m.matrix[1][2] + i.z*m.matrix[2][2] + m.matrix[3][2];
	float w = i.x *m.matrix[0][3] + i.y*m.matrix[1][3] + i.z*m.matrix[2][3] + m.matrix[3][3];

	if (w != 0.0f)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}
}
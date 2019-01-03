#pragma once

#include <vector>
#include <list>
#include <string.h>
#include <iostream>
#include  <fstream>
#include <strstream>

struct vector3f {
	float x, y, z, w;
	vector3f()
	{
		x = y = z =  0;
		w = 1;
	}
	vector3f(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
		w = 1;
	}

	float length()
	{
		return sqrtf(x*x + y * y + z * z);
	}

	vector3f normalized()
	{
		float l = length();
		vector3f nv = { x / l, y / l, z / l };
		return nv;
	}

	float dotProd(vector3f &vB)
	{
		return (x * vB.x + y * vB.y + z * vB.z);
	}

	float angle(vector3f &vB)
	{
		return acosf((x*vB.x + y * vB.y + z * vB.z) / (length()*vB.length()));
	}

	vector3f coross(vector3f& vB)
	{
		return{ y*vB.z - z * vB.y, z*vB.x - x * vB.z, x*vB.y - y * vB.x };
	}

	vector3f& operator += (vector3f& vB)
	{
		this->x += vB.x;
		this->y += vB.y;
		this->z += vB.z;
		return *this;
	}

	vector3f& operator -= (vector3f& vb)
	{
		this->x -= vb.x;
		this->y -= vb.y;
		this->z -= vb.z;
		return *this;
	}

	vector3f& operator *= (float value)
	{
		this->x *= value;
		this->y *= value;
		this->z *= value;
		return *this;
	}

	vector3f& operator /= (float value)
	{
		this->x /= value;
		this->y /= value;
		this->z /= value;
		return *this;
	}

	vector3f operator+(vector3f& vb)
	{
		vector3f nv = { this->x + vb.x, this->y + vb.y, this->z + vb.z };
		return nv;
	}

	vector3f operator-(vector3f& vb)
	{
		vector3f nv = { this->x - vb.x, this->y - vb.y, this->z - vb.z };
		return nv;
	}

	vector3f operator*(vector3f& vb)
	{
		vector3f nv = { this->x*vb.x, this->y*vb.y, this->z*vb.z };
		return nv;
	}

	vector3f operator/(vector3f& vb)
	{
		vector3f nv = { this->x / vb.x, this->y / vb.y, this->z / vb.z };
		return nv;
	}
};
struct triangle{
	vector3f vertices[3];
};

struct mesh {
	std::vector<triangle> triangles;

	bool LoadFromObjFile(std::string fileName)
	{
		std::ifstream f(fileName);
		if (!f.is_open())
			return false;

		std::vector<vector3f> vertices;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				vector3f v;
				s >> junk >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}

			if (line[0] == 'f')
			{
				int i[3];//vertices indexes
				s >> junk >> i[0] >> i[1] >> i[2];
				triangles.push_back({ vertices[i[0] - 1], vertices[i[1] - 1], vertices[i[2] - 1] });
			}
		}

		return true;
	}
};

struct matrix4x4 {
	float matrix[4][4] = { 0 };
};

matrix4x4 MatrixRotationZ(float a)
{
	matrix4x4 nm;
	nm.matrix[0][0] = cosf(a);
	nm.matrix[0][1] = sinf(a);
	nm.matrix[1][0] = -sinf(a);
	nm.matrix[1][1] = cosf(a);
	nm.matrix[2][2] = 1.0f;
	nm.matrix[3][3] = 1.0f;
	return nm;
}

matrix4x4 MatrixRotationY(float a)
{
	matrix4x4 nm;
	nm.matrix[0][0] = cosf(a);
	nm.matrix[0][2] = sinf(a);
	nm.matrix[2][0] = -sinf(a);
	nm.matrix[1][1] = 1.0f;
	nm.matrix[2][2] = cosf(a);
	nm.matrix[3][3] = 1.0f;
	return nm;
}

matrix4x4 MatrixRotationX(float a)
{
	matrix4x4 nm;
	nm.matrix[0][0] = 1.0f;
	nm.matrix[1][1] = cosf(a);
	nm.matrix[1][2] = sinf(a);
	nm.matrix[2][1] = -sinf(a);
	nm.matrix[2][2] = cosf(a);
	nm.matrix[3][3] = 1.0f;
	return nm;
}

matrix4x4 MatrixTranslation(float x, float y, float z)
{
	matrix4x4 nm;
	nm.matrix[0][0] = 1.0f;
	nm.matrix[1][1] = 1.0f;
	nm.matrix[2][2] = 1.0f;
	nm.matrix[3][3] = 1.0f;
	nm.matrix[3][0] = x;
	nm.matrix[3][1] = y;
	nm.matrix[3][2] = z;
	return nm;
}

matrix4x4 ScreenProjectionMatrix(float fov, float aRatio, float nearP, float farP)
{
	float fovRad = 1.0f / tanf(fov*0.5f / 180.0f * 3.14159f);
	matrix4x4 nm;
	nm.matrix[0][0] = aRatio * fovRad;
	nm.matrix[1][1] = fovRad;
	nm.matrix[2][2] = farP / (farP - nearP);
	nm.matrix[3][2] = (-farP * nearP) / (farP - nearP);
	nm.matrix[2][3] = 1.0f;
	nm.matrix[3][3] = 0.0f;
	return nm;
}


matrix4x4 MatrixMultMatrix(matrix4x4 &m1, matrix4x4 &m2)
{
	matrix4x4 nm;
	for (int c = 0; c < 4; c++)
	{
		for (int r = 0; r < 4; r++)
		{
			nm.matrix[r][c] = m1.matrix[r][0] * m2.matrix[0][c] + m1.matrix[r][1] * m2.matrix[1][c] + m1.matrix[r][2] * m2.matrix[2][c] + m1.matrix[r][3] * m2.matrix[3][c];
		}
	}
	return nm;
}

matrix4x4 MatrixIdentity()
{
	matrix4x4 nm;
	nm.matrix[0][0] = 1.0f;
	nm.matrix[1][1] = 1.0f;
	nm.matrix[2][2] = 1.0f;
	nm.matrix[3][3] = 1.0f;
	return nm;
}


vector3f MatrixMultVector3f(matrix4x4 &m, vector3f &v)
{
	vector3f nv;
	nv.x = v.x*m.matrix[0][0] + v.y*m.matrix[1][0] + v.z*m.matrix[2][0] + v.w*m.matrix[3][0];
	nv.y = v.x*m.matrix[0][1] + v.y*m.matrix[1][1] + v.z*m.matrix[2][1] + v.w*m.matrix[3][1];
	nv.z = v.x*m.matrix[0][2] + v.y*m.matrix[1][2] + v.z*m.matrix[2][2] + v.w*m.matrix[3][2];
	nv.w = v.x*m.matrix[0][3] + v.y*m.matrix[1][3] + v.z*m.matrix[2][3] + v.w*m.matrix[3][3];
	return nv;
}
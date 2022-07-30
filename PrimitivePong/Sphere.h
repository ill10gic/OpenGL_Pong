#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();

	Sphere(GLfloat prec);

	Sphere(GLfloat prec, float rad); // Todo - possible to implement with radius?

	/// <summary>
	/// An init method to actually generate the indices and verticies (and soon normals and texture coords)
	/// takes in a precision
	/// </summary>
	void Init(unsigned int prec);

protected:
	float radius;
	float precision;
};


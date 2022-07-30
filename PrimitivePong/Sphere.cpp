#include "Sphere.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm/glm.hpp>

Sphere::Sphere() {
	Init(48);
}

Sphere::Sphere(GLfloat prec, float rad)
{
	Sphere::Init(prec);
}

void Sphere::Init(unsigned int prec)
{
	
	numVertices = (prec + 1) * (prec + 1);
}

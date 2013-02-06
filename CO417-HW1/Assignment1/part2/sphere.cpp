#include <iostream>
#include <cmath>
#include "sphere.hpp"

using namespace std;

Sphere::Sphere(int diameter) {
    this->diameter = diameter;
    normals = new float[diameter * diameter * 3];
    float x, y, z;
    float r = diameter/2;
    for (int i = 0; i < diameter; i++) {
	for (int j = 0; j < diameter; j++) {
	    int index = i * 3  + j * 3 * diameter;
	    x = (float)(i - r);
	    y = (float)(r - j);
	    if (x * x + y * y <= r * r) {
		z = sqrt(r * r - x * x - y * y);
/*		normals[index] = norm(static_cast<float>(x)/sqrt(x*x + y*y + z*z));
		normals[index + 1] = norm(static_cast<float>(y)/sqrt(x*x + y*y + z*z));
		normals[index + 2] = norm(z/sqrt(x*x + y*y + z*z));*/

		normals[index] = static_cast<float>(x)/sqrt(x*x + y*y + z*z);
		normals[index + 1] = static_cast<float>(y)/sqrt(x*x + y*y + z*z);
		normals[index + 2] = z/sqrt(x*x + y*y + z*z);
	    }
	    else {
		normals[index] = 0.0f;
		normals[index + 1] = 0.0f;
		normals[index + 2] = 0.0f;
	    }
	}
    }
}

Sphere::~Sphere() {
    delete[] normals;
}

float Sphere::norm(float value) {
    return (value + 1)/2;
}

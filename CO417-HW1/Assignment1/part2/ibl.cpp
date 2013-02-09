#define _USE_MATH_DEFINES
#include <cmath>
#include "ibl.hpp"

IBL::IBL(ImageData<float> &latlong, Sphere &sphere) {

    float* r = new float[sphere.diameter * sphere.diameter * 3];
    float* res = new float[sphere.diameter * sphere.diameter * 3];
    
    float* v = new float[3];
    v[0] = 0;
    v[1] = 0;
    v[2] = 1;

    float theta, phi;

    for (int i = 0; i < sphere.diameter; ++i) {
	for (int j = 0; j < sphere.diameter; ++j) {
	    int index_s = i*sphere.diameter*3 + j*3;
	    float dot_p = dot(v, sphere.normals + index_s);
	    r[index_s] = (2 * dot_p * sphere.normals[index_s] - v[0]);
	    r[index_s + 1] = (2 * dot_p * sphere.normals[index_s + 1] - v[1]);
	    r[index_s + 2] = (2 * dot_p * sphere.normals[index_s + 2] - v[2]);
	    theta = acos(r[index_s + 1]);
		phi = atan2(sphere.normals[index_s + 2], sphere.normals[index_s]) + M_PI;
	    int index_l = floor((theta/M_PI) * latlong.height) * latlong.width * 3 + floor(phi/(2 * M_PI) * latlong.width ) * 3;
	    res[index_s] = latlong.data[index_l];
	    res[index_s + 1] = latlong.data[index_l + 1];
	    res[index_s + 2] = latlong.data[index_l + 2];
	}
    }
    ibl = new ImageData<float>(sphere.diameter, sphere.diameter, 3, res);
    this->reflection = new ImageData<float>(sphere.diameter, sphere.diameter, 3, r);

}


float IBL::dot(float* vec1, float* vec2) {
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

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
	    phi = atan2(r[index_s + 2], r[index_s]) + M_PI;
	    int index_l = floor((theta/M_PI) * latlong.height) * latlong.width * 3 + floor(phi/(2 * M_PI) * latlong.width ) * 3;
	    if (pow((i - sphere.diameter/2), 2) + pow((sphere.diameter/2) - j, 2) <= pow(sphere.diameter/2, 2)) {
		res[index_s] = latlong.data[index_l];
		res[index_s + 1] = latlong.data[index_l + 1];
		res[index_s + 2] = latlong.data[index_l + 2];
	    }
	    else {
		res[index_s] = 0;
		res[index_s + 1] = 0;
		res[index_s + 2] = 0;
	    }
	}
    }
    ibl = new ImageData<float>(sphere.diameter, sphere.diameter, 3, res);
    this->reflection = new ImageData<float>(sphere.diameter, sphere.diameter, 3, r);

}


float IBL::dot(float* vec1, float* vec2) {
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

void IBL::tonMapping(ImageData<float>* img) {
    int gamma = 2;
    int stops = 1;
    for ( int i = 0 ; i < img->height ; ++i ) // height
	{
	    for ( int j = 0 ; j < img->width ; ++j ) // width
	    {
			int index = i*img->width*img->numComponents + j*img->numComponents; //index within the image
			float n_R = (img->data[index] * pow(2.0,stops) > 1.0) ? 1.0 : img->data[index] * pow(2,stops) ;
			float n_G = (img->data[index + 1] * pow(2.0,stops) > 1.0) ? 1.0 : img->data[index + 1] * pow(2,stops) ;
			float n_B = (img->data[index + 2] * pow(2.0,stops) > 1.0) ? 1.0 : img->data[index + 2] * pow(2,stops) ;
			img->data[index] = pow(n_R,1/gamma);
			img->data[index + 1] = pow(n_G, 1/gamma);
			img->data[index + 2] = pow(n_B, 1/gamma);
		}
	 }
}

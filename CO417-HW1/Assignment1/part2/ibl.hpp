#ifndef __IBL__HPP__
#define __IBL_HPP__

#include "image.hpp"
#include "sphere.hpp"

class IBL {
public:

    IBL(ImageData<float> &latlong, Sphere &sphere);

    static void tonMapping(ImageData<float>* img);
    
    ImageData<float> *ibl;
    ImageData<float> *reflection;

private:
    float dot(float* vec1, float* vec2);
};

#endif

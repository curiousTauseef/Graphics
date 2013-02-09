#include <cmath>
#include "image.hpp"
#include "hdrimage.hpp"

int main(int agrc, char** argv) {

    HDRImage h = HDRImage("../Memorial/");
	h.save("derp");
	ImageData<unsigned char> t_map = ImageData<float>::convert<unsigned char>(h);
    
    t_map.save("derp");
	// exposure
	int stops = 9;
	float gamma = 2.2;
	for ( int i = 0 ; i < h.height ; ++i ) // height
	{
	    for ( int j = 0 ; j < h.width ; ++j ) // width
	    {
		    int index = i*h.width*h.numComponents + j*h.numComponents; //index within the image
			float n_R = (h.data[index] * pow(2.0,stops) > 1.0) ? 1.0 : h.data[index] * pow(2,stops) ;
			float n_G = (h.data[index + 1] * pow(2.0,stops) > 1.0) ? 1.0 : h.data[index + 1] * pow(2,stops) ;
			float n_B = (h.data[index + 2] * pow(2.0,stops) > 1.0) ? 1.0 : h.data[index + 2] * pow(2,stops) ;
			h.data[index] = pow(n_R,1/gamma);
			h.data[index + 1] = pow(n_G, 1/gamma);
			h.data[index + 2] = pow(n_B, 1/gamma);
		}
	 }

	
	h.save("derp_exp");
    return 0;
}

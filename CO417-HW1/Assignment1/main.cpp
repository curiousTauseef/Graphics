#include "image.hpp"
#include "hdrimage.hpp"

int main(int agrc, char** argv) {

    HDRImage h = HDRImage("../Memorial/");
	h.save("derp");
	ImageData<unsigned char> t_map = ImageData<float>::convert<unsigned char>(h);
    
    t_map.save("derp");
	// exposure
	HDRImage::tonMapping(h, 7, 2.2);
	
	h.save("derp_exp");
	ImageData<unsigned char> t_map_exp = ImageData<float>::convert<unsigned char>(h);
    
    t_map_exp.save("derp_exp");
    return 0;
}

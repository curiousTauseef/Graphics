#include "image.hpp"
#include "hdrimage.hpp"

int main(int agrc, char** argv) {

    HDRImage h = HDRImage("../Memorial/");
    ImageData<unsigned char> t_map = ImageData<float>::convert<unsigned char>(h);
    t_map.save("derp");
    h.save("derp");
    return 0;
}

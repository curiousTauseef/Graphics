#include <iostream>
#include "sphere.hpp"
#include "image.hpp"
#include "ibl.hpp"

using namespace std;

int main(int argc, char** argv) {
    Sphere s(511);
    ImageData<float> ll = ImageData<float>::load("../../GraceCathedral/grace_latlong.pfm");
    IBL test(ll, s); 
    ImageData<float> *normals = new ImageData<float>(s.diameter, s.diameter, 3, s.normals);
    normals->save("normals");
    (test.reflection)->save("reflection");
    (test.ibl)->save("ibl");

    ImageData<unsigned char> normals_ppm = ImageData<float>::convert<unsigned char>(*normals);
    ImageData<unsigned char> reflection_ppm = ImageData<float>::convert<unsigned char>(*(test.reflection));
    ImageData<unsigned char> ibl_ppm = ImageData<float>::convert<unsigned char>(*(test.ibl));
    normals_ppm.save("normals");
    reflection_ppm.save("reflection");
    ibl_ppm.save("ibl");
    return 0;
}

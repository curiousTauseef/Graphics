#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>

#include "image.hpp"
#include "hdrimage.hpp"

using namespace std;

HDRImage::HDRImage(string path) {
    float div = 4;
    stringstream s_buf;
    s_buf << path << "/memorial1.pfm";
    ImageData<float> F = ImageData<float>::load(s_buf.str());
    fill_n(F.data, F.width * F.height * F.numComponents, 0.0f);
    vector<ImageData<float>> Zi;
    float F_R, F_G, F_B;
    for (int i = 1; i <= 7; ++i) {
	stringstream s_str;
	s_str << path << "/memorial" << i << ".pfm";
	string path = s_str.str();
	ImageData<float> img = ImageData<float>::load(path.c_str());
	Zi.push_back(img);
    }

    for ( int i = 0 ; i < F.height ; ++i ) // height
    {
	for ( int j = 0 ; j < F.width ; ++j ) // width
	{
	    for ( int k = 0 ; k < F.numComponents ; ++k ) // color channels - 3 for RGB images
	    {
		int index = i*F.width*F.numComponents + j*F.numComponents + k; //index within the image
		float sum_Zi_R = 0.0f;
		float sum_Zi_G = 0.0f;
		float sum_Zi_B = 0.0f;
		float div = 1.0f;
		for (int img = 1; img <= 7; ++img) {
		    float Zi_R = Zi.at(img - 1).data[index];
		    sum_Zi_R += weight(Zi_R);
		    float Zi_G = Zi.at(img - 1).data[index + 1];
		    sum_Zi_G += weight(Zi_G);
		    float Zi_B = Zi.at(img -1).data[index + 2];
		    sum_Zi_B += weight(Zi_B);
		    F.data[index] += log(Zi_R/div) * weight(Zi_R);
		    F.data[index + 1] += log(Zi_G/div) * weight(Zi_G);
		    F.data[index + 2] += log(Zi_B/div) * weight(Zi_B);
		    div *= 4;
		}
		F_R = exp(F.data[index]/sum_Zi_R);
		F_G = exp(F.data[index + 1]/sum_Zi_G);
		F_B = exp(F.data[index + 2]/sum_Zi_B);

		F.data[index] = F_R <= 1.0f ? (F_R >= 0.0f ? F_R : 0.0f) : 1.0f;
		F.data[index + 1] = F_G <= 1.0f ? (F_G >= 0.0f ? F_G : 0.0f) : 1.0f;
		F.data[index + 2] = F_B <= 1.0f ? (F_B >= 0.0f ? F_B : 0.0f) : 1.0f;
	    }
	    if (i == 85 && j == 189) {
		cout << "derp:" << F_R << " " << F_G << " " << F_B << endl;
	    }
	    
	}
    }
    data = F.data;
    width = F.width;
    height = F.height;
    numComponents = F.numComponents;
}

float HDRImage::weight(float value) {
    return (value >= 0.005 && value <= 0.92) ? 2 * (8 * pow(value, 4) - 16 * pow(value, 3) + 8 * pow(value, 2)) : 0.0f;
//    return 2 * (8 * pow(value, 4) - 16 * pow(value, 3) + 8 * pow(value, 2));
}

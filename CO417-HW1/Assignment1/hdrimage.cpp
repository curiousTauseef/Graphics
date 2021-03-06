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
    float brightest = 0.0f;
    float dimmest = 1.0f;
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
	    //for ( int k = 0 ; k < F.numComponents ; ++k ) // color channels - 3 for RGB images
	    //{
		int index = i*F.width*F.numComponents + j*F.numComponents ; //index within the image
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
		    F.data[index] += (Zi_R < 0.005f || Zi_R > 0.92f) ? 0.0f : log(Zi_R/div) * weight(Zi_R);
		    F.data[index + 1] += (Zi_G < 0.005f || Zi_G > 0.92f) ? 0.0f : log(Zi_G/div) * weight(Zi_G);
		    F.data[index + 2] += (Zi_B < 0.005f || Zi_B > 0.92f) ? 0.0f : log(Zi_B/div) * weight(Zi_B);
		    div *= 4.0f;
		}
		F_R = (sum_Zi_R == 0.0f) ? 1.0f : exp(F.data[index]/sum_Zi_R);
		F_G = (sum_Zi_G == 0.0f) ? 1.0f : exp(F.data[index + 1]/sum_Zi_G);
		F_B = (sum_Zi_B == 0.0f) ? 1.0f : exp(F.data[index + 2]/sum_Zi_B);

		F.data[index] = F_R;
		F.data[index + 1] = F_G;
		F.data[index + 2] = F_B;
		float av = (F_R + F_G + F_B)/3;
		if (av != 0 && av <= dimmest)
		    dimmest = av;
		if (av >= brightest)
		    brightest = av;
	    //}
	}
    }
    cout << brightest/dimmest << endl;
    data = F.data;
    width = F.width;
    height = F.height;
    numComponents = F.numComponents;
}

float HDRImage::weight(float value) {
    float tmp = 4.0f * (value * value - value);
    return (value < 0.005f || value > 0.92f) ? 0.0f : tmp * tmp;
}

void HDRImage::tonMapping(HDRImage img, int stops, float gamma) {
	for ( int i = 0 ; i < img.height ; ++i ) // height
	{
	    for ( int j = 0 ; j < img.width ; ++j ) // width
	    {
			int index = i*img.width*img.numComponents + j*img.numComponents; //index within the image
			float n_R = (img.data[index] * pow(2.0,stops) > 1.0) ? 1.0 : img.data[index] * pow(2,stops) ;
			float n_G = (img.data[index + 1] * pow(2.0,stops) > 1.0) ? 1.0 : img.data[index + 1] * pow(2,stops) ;
			float n_B = (img.data[index + 2] * pow(2.0,stops) > 1.0) ? 1.0 : img.data[index + 2] * pow(2,stops) ;
			img.data[index] = pow(n_R,1/gamma);
			img.data[index + 1] = pow(n_G, 1/gamma);
			img.data[index + 2] = pow(n_B, 1/gamma);
		}
	 }
}

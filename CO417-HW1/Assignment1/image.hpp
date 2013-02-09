#ifndef __IMAGE__HPP__
#define __IMAGE__HPP__

#include <stdio.h>
#include <iostream>
#include <string>
#include <type_traits>

template <class T>
class ImageData {

public:
    ~ImageData() {
    }
    
    static ImageData<T> load(std::string path) {
	FILE          *imgFile;
	char          buf[1024];
	char          type;
	int           i, j, k, l;
	double        distance, x, y;
	unsigned int width, height, numComponents;
	T* imageData;
	T* tmpData;
	ImageData<T> img;

	imgFile= fopen( path.c_str(), "rb" );

	if (std::is_same<T, unsigned char>::value) {
	    if( fscanf( imgFile, "P%c \n", &type )!= 1 || type< '1' || type> '8')
	    {
		fclose( imgFile );
		
	    }
	    // skip comments
	    while( fscanf( imgFile, "#%[^\n]\n", buf ) )
		;
	    // read width
	    fscanf( imgFile, "%d", &width );
	    /* skip comments */
	    while( fscanf( imgFile, "#%[^\n]\n", buf ) )
		;
	    /* read height */
	    fscanf( imgFile, "%d", &height );
	    /* skip comments */
	    while( fscanf( imgFile, "#%[^\n]\n", buf ) )
		;
	    /* skip max. component and exactly one whitespace */
	    fscanf( imgFile, "%*d%*c" );

	    switch( type )
	    {
	    case '1': // ASCII bitmap
	    case '4': // binary bitmap
		std::cerr << "Bitmaps not implemented\n";
		fclose( imgFile );
		
	    case '2': // ASCII greymap
		imageData= new T[width*height*3];
		for( i= 0 ; i< height ; i++ )
		    for( j= 0 ; j< width ; j++ )
		    {
			fscanf( imgFile, "%d", &l );
			imageData[i*width+j]= l;
		    }
		numComponents= 1;
		break;
	    case '3': // ASCII RGB
		imageData= new T[width*height*3];
		for( i= 0 ; i< height ; i++ )
		    for( j= 0 ; j< width ; j++ )
			for( k= 0 ; k< 3 ; k++ )
			{
			    fscanf( imgFile, "%d", &l );
			    imageData[(i*width+j)*3+k]= l;
			}
		numComponents= 3;
		break;
	    case '5': // binary greymap
		imageData= new T[width*height];
		fread( imageData, 1, width*height, imgFile );
		numComponents= 1;
		break;
	    case '6': // binary RGB
		imageData= new T[width*height*3];
		fread( imageData, 1, width*height*3, imgFile );
		numComponents= 3;
		break;
	    }
	}
	else {
	    fscanf( imgFile, "PF \n", &type );
	    // skip comments
	    while( fscanf( imgFile, "#%[^\n]\n", buf ) )
		;

	    // read width
	    fscanf( imgFile, "%d", &width );
	    /* skip comments */
	    while( fscanf( imgFile, "#%[^\n]\n", buf ) )
		;   
  
	    /* read height */
	    fscanf( imgFile, "%d", &height );
	    /* skip comments */
	    while( fscanf( imgFile, "#%[^\n]\n", buf ) )
		;

   
    
	    /* skip max. component and exactly one whitespace */
	    fscanf( imgFile, "%*f%*c" );


  
	    imageData= new T[width*height*3];
  
	    fread( imageData, sizeof(T), width*height*3, imgFile );
	    numComponents= 3;

	    fclose( imgFile );
  
	    tmpData= new T[width*3];
  
	    //invert image for reading!!!
	    for(int i=0;i<height/2;i++)
	    {
		for(int j=0;j<width;j++)
		{
		    for(int k=0;k<3;k++)
		    {
			int indexS = i*width*3 + j*3 + k;
			int indexD = (height-1 - i)*width*3 + j*3 + k;
      
			tmpData[j*3 + k] = imageData[indexS];
			imageData[indexS] = imageData[indexD];
			imageData[indexD] = tmpData[j*3 + k];           
		    }
		} 
	    }
  
	}
	delete[] tmpData;
	img.data = imageData;
	img.width = width;
	img.height = height;
	img.numComponents = numComponents;
	return img;
    }

    void save(std::string name) {
	FILE          *fp;
	char          buf[1024];
	char          type;
	int           i, j, k, l;
	double        distance, x, y;
	float max=1.0f;
	char space=' ';

// Write PGM image file with filename "file"

// The PGM file format for a GREYLEVEL image is:
// P5 (2 ASCII characters) <CR>
// two ASCII numbers for nx ny (number of rows and columns <CR>
// 255 (ASCII number) <CR>
// binary pixel data with one byte per pixel

// The PGM file format for a COLOR image is:
// P6 (2 ASCII characters) <CR>
// two ASCII numbers for nx ny (number of rows and columns <CR>
// 255 (ASCII number) <CR>
// binary pixel data with three bytes per pixel (one byte for each R,G,B)

	if (std::is_same<T, float>::value) {
	    fp=fopen((name + ".pfm").c_str(),"wb");
	    fputc('P', fp);
	    fputc('F', fp);
	    fputc(0x0a, fp);
	    
	    fprintf(fp, "%d %d", width, height);
	    fputc(0x0a, fp);
	    
	    fprintf(fp, "%f", -1.0f);
	    fputc(0x0a, fp);
	    for(i=height-1;i>=0;i--)
		fwrite(&data[i*width*numComponents],sizeof(float),width*numComponents, fp);
	}
	else {
	    fp=fopen((name + ".ppm").c_str(),"wb");
	    // write the first ASCII line with the file type
	    if(numComponents==1)
		fprintf(fp,"P5\n"); //greylevel image
	    else if(numComponents==3)
		fprintf(fp,"P6\n");  // color image
	
	    // write image dimensions
	    fprintf(fp,"%d %d\n",width,height);
	
	    // write '255' to the next line
	    fprintf(fp,"255\n");
	
	    fwrite(data,sizeof(unsigned char),width*height*numComponents, fp);

	}

	fclose(fp);
    }

    template <class T1>
    static ImageData<T1> convert(ImageData<T> img) {
	ImageData<T1> ret;
	T1 *img_out = new T1[img.width*img.height*img.numComponents];
	for ( int i = 0 ; i < img.height ; ++i ) // height
	{
	    for ( int j = 0 ; j < img.width ; ++j ) // width
	    {
		//for ( int k = 0 ; k < img.numComponents ; ++k ) // color channels - 3 for RGB images
		//{
		    int index = i*img.width*img.numComponents + j*img.numComponents; //index within the image
		    if (std::is_same<T1, unsigned char>::value) {
			//typecast 0.0f -> 1.0f values to the 0 - 255 range 
			img_out[index] = static_cast<unsigned char>(img.data[index]*255.0f); //R
			img_out[index + 1] = static_cast<unsigned char>(img.data[index + 1]*255.0f);//G
			img_out[index + 2] = static_cast<unsigned char>(img.data[index + 2]*255.0f);//B
			
		    }
		    else {
			//typecast 0 - 255 values to the 0.0f -> 1.0f range 
			img_out[index] = static_cast<float>(img.data[index])/255.0f; //R
			img_out[index + 1] = static_cast<float>(img.data[index + 1])/255.0f;//G
			img_out[index + 2] = static_cast<float>(img.data[index + 2])/255.0f;//B
			}
		//}
		
	    }
	}
	ret.width = img.width;
	ret.height = img.height;
	ret.numComponents = img.numComponents;
	ret.data = img_out;
	return ret;
    }

    ImageData() {}

//protected:
    T* data;
    unsigned int width;
    unsigned int height;
    unsigned int numComponents;
};

#endif

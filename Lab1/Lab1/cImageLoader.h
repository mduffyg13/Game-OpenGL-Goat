/*
==========================================================================
cImageLoader.h
==========================================================================
*/

#ifndef _IMAGELOADER_H
#define _IMAGELOADER_H

#include "GameConstants.h"
#include <windows.h>
#include "FreeImageDist\Dist\FreeImage.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

class cImageLoader
{
private:

	FIBITMAP *m_dib;  //pointer to the image, once loaded
	BYTE* m_ImageData;  //pointer to the image data
	unsigned int m_ImageWidth;  // image width
	unsigned int m_ImageHeight; // image height

public:
	
	cImageLoader();			// Constructor
	cImageLoader(const char* filename);  // Constructor
	~cImageLoader();        // destructor

	FIBITMAP* LoadTexture(const char* filename);	//where to load the file from
	
	BYTE* getImageData();
	void setImageData(FIBITMAP* texture);

	unsigned int getImageWidth();
	void setImageWidth(FIBITMAP* texture);

	unsigned int getImageHeight();
	void setImageHeight(FIBITMAP* texture);

	//free the memory for a texture
	void unloadTexture(FIBITMAP *textureToUnload);
	GLuint loadDDS(const char * imagepath);

};
#endif
#define _CRT_SECURE_NO_DEPRECATE
/*
==================================================================================
cImageLoader.cpp
==================================================================================
*/

#include "cImageLoader.h"
//#include "Images\"

cImageLoader::cImageLoader()				// Constructor
{
	m_dib = NULL;
	m_ImageData = NULL;
	m_ImageWidth = 0;
	m_ImageHeight = 0;
}

cImageLoader::cImageLoader(const char* filename)
{
	m_dib = LoadTexture(filename);
}

cImageLoader::~cImageLoader()        // destructor
{
	//unloadTexture(m_dib);
}

FIBITMAP* cImageLoader::LoadTexture(const char* filename)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return NULL;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return NULL;

	setImageData(dib);
	setImageWidth(dib);
	setImageHeight(dib);

	bits = getImageData();
	width = getImageWidth();
	height = getImageHeight();
	
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return NULL;

	//return success
	m_dib = dib;
	return dib;
}
BYTE* cImageLoader::getImageData()
{
	//retrieve the image data
	return m_ImageData;
}

void cImageLoader::setImageData(FIBITMAP* texture)
{
	m_ImageData = FreeImage_GetBits(texture);
}

unsigned int cImageLoader::getImageWidth()
{
	//get the image width
	return m_ImageWidth;
}

void cImageLoader::setImageWidth(FIBITMAP* texture)
{
	m_ImageWidth = FreeImage_GetWidth(texture); //get the image width

}

unsigned int cImageLoader::getImageHeight()
{
	return m_ImageHeight;
}

void cImageLoader::setImageHeight(FIBITMAP* texture)
{
	m_ImageHeight = FreeImage_GetHeight(texture);
}

//free the memory for a texture
void cImageLoader::unloadTexture(FIBITMAP *textureToUnload)
{
	//Free FreeImage's copy of the data
	FreeImage_Unload(textureToUnload);
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint cImageLoader::loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;


}
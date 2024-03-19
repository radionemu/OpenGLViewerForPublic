#ifndef TEXTURELOAD_HPP
#define TEXTURELOAD_HPP

#include <GL/glew.h>

GLuint TextureBind(int width, int height, const void * buffer);

GLuint LoadPNG(const char * PNGpath);

GLuint LoadPNG_Mem(const unsigned char * PNGpath, size_t insize);

GLuint LoadJPG(const char * JPGpath);

GLuint LoadJPG_Mem(const unsigned char * JPGpath, size_t insize);

GLuint LoadDDS(const char * DDSpath);

#endif
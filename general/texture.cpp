#include <GL/glew.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.hpp"

#define FILEHEADER 0x89504E470D0A1A0A
#define GLV_RGB 03
#define GLV_RGBA 04

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
#define FOURCC_BPTC 0x30315844
#define FOURCC_BC4 0x53344342
#define DDPF_FOURCC 0x00000004

static void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError(){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] : " << error << std::endl;
    }
}

GLuint TextureBind(int width, int height, const void * buffer, unsigned int type){
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    printf("Texture WIDTH : %d HEIGHT : %d\n", width, height);
    GLClearError();
    if(type == GLV_RGBA){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);//GL_RGBA!!!!!!!!!!!!
        GLCheckError();
    }else if(type == GLV_RGB){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);//GL_RGBA!!!!!!!!!!!!
        GLCheckError();
    }

    // GLCheckError();

    //check it later
    GLClearError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	GLCheckError();

    GLClearError();
    // glGenerateMipmap(GL_TEXTURE_2D);
    GLCheckError();
    printf("textureID : %d", textureID);
    return textureID;
}

GLuint LoadPNG(const char * PNGpath){
    int width, height, nrChannels;
    unsigned char* data = stbi_load(PNGpath, &width, &height, &nrChannels, 0);
    if(!data){
        printf("PNG file [%s]open failed\n", PNGpath);getchar();
        return -1;
    }
    printf("PNG file [%s]opened.Color Type is %d\n", PNGpath, nrChannels);
    unsigned int txtureid = TextureBind(width, height, data, nrChannels);
    stbi_image_free(data);
    return txtureid;
}

GLuint LoadPNG_Mem(const uint8_t * BinaryPointer, size_t insize){
    int width, height, nrChannels;
    uint8_t* data = stbi_load_from_memory(BinaryPointer, insize, &width, &height, &nrChannels, 0);
    if(!data){
        printf("PNG open failed form Memory.\n");getchar();
        return -1;
    }
    printf("PNG file opened from Memory, Color Channel is %d\n", nrChannels);

    unsigned int textureid = TextureBind(width, height, data, nrChannels);
    stbi_image_free(data);
    return textureid;
}

// GLuint LoadPNG_Mem(const unsigned char * PNGpath, size_t insize){
//     //wait what
//     std::vector<unsigned char> PNGin(PNGpath, PNGpath+insize);
//     std::vector<unsigned char> PNGimage;
//     unsigned int width, height;
//     unsigned int res = lodepng::decode(PNGimage, width, height, PNGin);
//     //unsigned int res = lodepng_decode_memory(PNGimage, &width, &height, PNGpath, insize, LCT_RGBA, (unsigned int)8);
//     char * fullpath[PATH_MAX];

//     if(res != 0){
//         printf("PNG open failed from Memory. [Error Code : %d]\n", res);getchar();
//         return 1;
//     }

//     printf("PNG file opened from Memory.\n");
    
//     return TextureBind(width, height, &PNGimage[0], PNG);
// }

GLuint LoadJPG(const char* FileName)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(FileName, &width, &height, &nrChannels, 0);
    if(!data){
        printf("JPG file [%s]open failed", FileName);getchar();
        return 1;
    }
    printf("JPG file [%s]opened.\n", FileName);
    return TextureBind(width, height, &data[0], nrChannels);
}

typedef struct{
  unsigned int dwSize;
  unsigned int dwFlags;
  unsigned int dwFourCC;
  unsigned int dwRGBBitCount;
  unsigned int dwRBitMask;
  unsigned int dwGBitMask;
  unsigned int dwBBitMask;
  unsigned int dwABitMask;
}DDS_PIXELFORMAT;

typedef struct{
    unsigned int dwSize;
    unsigned int dwFlags;
    unsigned int dwHeight;
    unsigned int dwWidth;
    unsigned int dwPitchOrLinearSize;
    unsigned int dwDepth;
    unsigned int dwMipMapCount;
    unsigned int dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    unsigned int dwCaps;
    unsigned int dwCaps2;
    unsigned int dwCaps3;
    unsigned int dwCaps4;
    unsigned int dwReserved2;
}DDS_HEADER;

typedef struct {
  unsigned int dxgiFormat;
  unsigned int resourceDimension;
  unsigned int miscFlag;
  unsigned int arraySize;
  unsigned int miscFlags2;
} DDS_HEADER_DXT10;

GLuint LoadDDS(const char *DDSpath)
{
    unsigned char header[124];
 
    FILE *fp;
 
    /* try to open the file */
    fp = fopen(DDSpath, "rb");
    if (fp == NULL) {
        printf("DDS file [%s]open failed\n", DDSpath); getchar();
        return -1;
    }
 
    // DWORD
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        printf("DDS file [%s]open failed\n", DDSpath); getchar();
        return -1;
    }
 
    printf("DDS file [%s]open\n", DDSpath);

    // DDS_HEADER
    DDS_HEADER dh;
    DDS_HEADER_DXT10 dxt10;
    fread(&dh, 124, 1, fp); //checkpoint
    // printf("dwFlags : %d DDPF_FOURCC : %d\n", dh.dwFlags, DDPF_FOURCC);
    // printf("dwFourCC : %x \n", dh.ddspf.dwFourCC);
    if((dh.ddspf.dwFourCC) == 0x30315844){
        // printf("WELCOMETOTHENEXTLEVEL\n");
        fread(&dxt10, 20, 1, fp);
    }

    
    // printf("dwsize : %d\n", dh.dwSize);

    unsigned char * buffer;
    unsigned int bufsize;
 
    /* how big is it going to be including all mipmaps? */
    // printf("mipmapcount : %d\n linearsize : %d\n", dh.dwMipMapCount, dh.dwPitchOrLinearSize);
    dh.dwPitchOrLinearSize = dh.dwWidth * dh.dwHeight;
    bufsize = dh.dwMipMapCount > 1 ? dh.dwPitchOrLinearSize * 2 : dh.dwPitchOrLinearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
 
    // printf("bufsize : %d\n", bufsize);

    /* close the file pointer */
    fclose(fp);

    unsigned int components = 4;
    unsigned int format = GL_COMPRESSED_RGBA_BPTC_UNORM;
    unsigned int blockSize = 16;

	GLuint textureID;
	glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);



	/* load the mipmaps */ 
    unsigned int offset = 0;
	for (unsigned int level = 0; level < dh.dwMipMapCount && (dh.dwWidth || dh.dwHeight); ++level) 
	{ 
		unsigned int sizes = ((dh.dwWidth+3)/4)*((dh.dwHeight+3)/4)*blockSize;

        // printf("level : %d\n",level);
        // printf("format : %d\n",format);
        // printf("dwWidth : %d\n",dh.dwWidth);
        // printf("dwHeight : %d\n",dh.dwHeight);
        // printf("size : %d\n",sizes);
        GLClearError();
		glCompressedTexImage2D(GL_TEXTURE_2D, level, GL_COMPRESSED_RGBA_BPTC_UNORM_ARB, dh.dwWidth, dh.dwHeight,  
			0, sizes, (buffer + offset)); 
	    GLCheckError(); 
		offset += sizes; 
		dh.dwWidth  /= 2; 
		dh.dwHeight /= 2; 
		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(dh.dwWidth < 1) dh.dwWidth = 1;
		if(dh.dwHeight < 1) dh.dwHeight = 1;

	} 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // float aniso = 0.0f;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso); 
    // glGenerateMipmap(GL_TEXTURE_2D); 
    // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    free(buffer);

    printf("textureID : %d\n", textureID);

    return textureID;
}
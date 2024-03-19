#pragma once
#include <GL/glew.h>
#include "AbsBuffer.hpp"

class FrameBuffer : public AbsBuffer
{
private:
    GLuint BufferID = 0; //FrameBuffer
    GLuint ClrBufTexID; //ColorBuffer
    GLuint DptBufTexID; //DepthBuffer
    GLuint StcBufTexID; //StencilBuffer

    size_t mWidth;
    size_t mHeight;
public:
    FrameBuffer();
    ~FrameBuffer();
    void Bind() override;
    void Unbind() override;
    void initBuffer(size_t width, size_t height);
    void generateBuffer(size_t resolution);
    void resize(size_t nWidth, size_t nHeight);
    
    GLuint getTextureID(){return ClrBufTexID;};
    GLuint getWidth(){return mWidth;}
    GLuint getHeight(){return mHeight;}
};

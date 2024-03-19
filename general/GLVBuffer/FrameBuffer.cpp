#include "FrameBuffer.hpp"
#include <iostream>

//RESIZE :: WINSCREENSIZE_VER
//RESIZE :: SCENEVIEW_VER

FrameBuffer::FrameBuffer(){

}   

FrameBuffer::~FrameBuffer(){

}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, BufferID);
    glViewport(0,0,mWidth,mHeight);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::initBuffer(size_t width, size_t height)
{
    this->mWidth = width;
    this->mHeight = height;

    if(BufferID){
        glDeleteFramebuffers(1, &BufferID);
        glDeleteTextures(1, &ClrBufTexID);
        glDeleteTextures(1, &DptBufTexID);
        BufferID = 0;
        ClrBufTexID = 0;
        DptBufTexID = 0;
    }

    //Init Buffer
    glGenFramebuffers(1, &BufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, BufferID);

    //Color Buffer
    glGenTextures(1, &ClrBufTexID);
    glBindTexture(GL_TEXTURE_2D, ClrBufTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight,0,GL_RGBA,GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Depth Buffer
    glGenTextures(1, &DptBufTexID);
    glBindTexture(GL_TEXTURE_2D, DptBufTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ClrBufTexID,0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, DptBufTexID, 0);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
        std::cout<<"Frame Buffer error : " << fboStatus << std::endl;
    }

    //????????????????????????????????????????????????????????//
    static const GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    glBindFramebuffer(GL_FRAMEBUFFER,0);

}

void FrameBuffer::generateBuffer(size_t resolution)
{
    this->mWidth = resolution;
    this->mHeight = resolution;

    if(BufferID){
        glDeleteFramebuffers(1, &BufferID);
        // glDeleteTextures(1, &ClrBufTexID);
        BufferID = 0;
        ClrBufTexID = 0;//only reset id
        DptBufTexID = 0;//only reset id
    }

    //Init Buffer
    glGenFramebuffers(1, &BufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, BufferID);

    //Color Buffer
    glGenTextures(1, &ClrBufTexID);
    glBindTexture(GL_TEXTURE_2D, ClrBufTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight,0,GL_RGBA,GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ClrBufTexID,0);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
        std::cout<<"Frame Buffer error : " << fboStatus << std::endl;
    }

    //????????????????????????????????????????????????????????//
    static const GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::resize(size_t nWidth, size_t nHeight)
{
    this->mWidth = nWidth;
    this->mHeight = nHeight;
    glViewport(0,0, mWidth, mHeight);
}

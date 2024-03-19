#pragma once
#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include <GL/glew.h>
#include "AbsBuffer.hpp"

class IndexBuffer : public AbsBuffer
{
    private:
        GLuint BufferID;//VBO needs Buffer's ID to Bind and Buffer Data
    public:
        IndexBuffer();
        IndexBuffer(const void* data, unsigned long long size);
        ~IndexBuffer();

        void Bind();
        void Unbind();
        void FillData(const void* data, unsigned long long size);
};

#endif
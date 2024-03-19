#pragma once
#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <GL/glew.h>
#include "AbsBuffer.hpp"

class VertexBuffer : public AbsBuffer
{
    private:
        GLuint BufferID;//VBO needs Buffer's ID to Bind and Buffer Data
    public:
        VertexBuffer();
        VertexBuffer(void const* data, uint64_t size);
        ~VertexBuffer();

        void Bind();
        void Unbind();
        void FillData(void const * data, uint64_t size);
};

#endif
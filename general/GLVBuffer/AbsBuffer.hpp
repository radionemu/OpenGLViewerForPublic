#pragma once
#ifndef ABS_BUFFER_HPP
#define ABS_BUFFER_HPP

#include <GL/glew.h>

class AbsBuffer
{
    private:
        GLuint BufferID;//VBO needs Buffer's ID to Bind and Buffer Data
    public:
        AbsBuffer();
        AbsBuffer(void const* data, uint64_t size);
        ~AbsBuffer();

        virtual void Bind() =0;
        virtual void Unbind() =0;
};

#endif
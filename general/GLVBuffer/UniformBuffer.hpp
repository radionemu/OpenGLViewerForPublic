#pragma once
#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include <GL/glew.h>
#include "AbsBuffer.hpp"
#include <vector>

class UniformBuffer : public AbsBuffer
{
    private:
        GLuint BufferID;//VBO needs Buffer's ID to Bind and Buffer Data
        unsigned int ubosize = 0;

        //Uniform only variables
        std::vector<std::pair<const void *, unsigned long long>> dataList;
    public:
        UniformBuffer();
        UniformBuffer(const void* data, unsigned long long size);
        ~UniformBuffer();

        void Bind();
        void Unbind();
        void AddData(const void* data, unsigned long long size);
        void FillData(unsigned int binding);
};

#endif
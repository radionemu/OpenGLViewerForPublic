#include "IndexBuffer.hpp"
#include <stdio.h>
// generate Buffer and Bind buffer.
// This Constructor does not alloc Data
IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &BufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
}

// generate Buffer, Bind Buffer, and Data Buffer
// size must be calculated as byte, not sizeof elements
IndexBuffer::IndexBuffer(const void* data, unsigned long long size)
{
    IndexBuffer();
    FillData(data, size);
}

IndexBuffer::~IndexBuffer()
{
    Unbind();
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
}

void IndexBuffer::Unbind()
{
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//0 == unbind
}

void IndexBuffer::FillData(const void* data, unsigned long long size)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

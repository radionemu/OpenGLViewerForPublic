#include "VertexBuffer.hpp"
#include "AbsBuffer.hpp"
#include <stdio.h>
// generate Buffer and Bind buffer.
// This Constructor does not alloc Data
VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &BufferID);
    glBindBuffer(GL_ARRAY_BUFFER, BufferID);
}

// generate Buffer, Bind Buffer, and Data Buffer
// size must be calculated as byte, not sizeof elements
VertexBuffer::VertexBuffer(void const * data, uint64_t size)
{
    VertexBuffer();
    FillData(data, size);
}

VertexBuffer::~VertexBuffer()
{
    // Unbind();
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferID);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);//0 == unbind
}

void VertexBuffer::FillData(void const* data, uint64_t size)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    printf("Buffer SIZE : %d\n", size);
}

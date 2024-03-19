#include "UniformBuffer.hpp"
#include <vector>

UniformBuffer::UniformBuffer()
{
    glGenBuffers(1, &BufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, BufferID);
}

UniformBuffer::UniformBuffer(const void *data, unsigned long long size)
{
    UniformBuffer();
    AddData(data, size);
    FillData(0);
}

UniformBuffer::~UniformBuffer()
{
    Unbind();
}

void UniformBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, BufferID);
}

void UniformBuffer::Unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::AddData(const void *data, unsigned long long size)
{
    std::pair<const void *, unsigned long long> P = {data, size};
    this->dataList.emplace_back(P);
    ubosize += size;
}

//This function doesn't works well on massive uniform data
//Because every time we have to 
void UniformBuffer::FillData(unsigned int binding)
{
    unsigned long long subDataIndex = 0;

    glBufferData(GL_UNIFORM_BUFFER, ubosize, NULL, GL_STATIC_DRAW);
    for(int i=0; i<this->dataList.size(); i++){
        glBufferSubData(GL_UNIFORM_BUFFER, subDataIndex, this->dataList[i].second, this->dataList[i].first);
        subDataIndex +=this->dataList[i].second;
    }
    glBindBufferRange(GL_UNIFORM_BUFFER, binding, BufferID, 0, ubosize);
}

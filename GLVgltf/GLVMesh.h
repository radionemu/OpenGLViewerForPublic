#pragma once

#include <fx/gltf.h>
#include "MeshData.h"
#include "MaterialData.h"
#include <glm/glm.hpp>
#include <vector>
#include "general/GLVBuffer/VertexBuffer.hpp"
#include "general/GLVVertexArray/BufferLayout.hpp"
#include "general/GLVVertexArray/VertexArray.hpp"
#include "general/GLVBuffer/IndexBuffer.hpp"

class GLVMesh{
    public:
        int mimeType(std::string str);
        bool Create(fx::gltf::Document const & doc, std::size_t meshIndex, std::vector<VertexArray> *vao, unsigned int programID, std::filesystem::path rootpath);
        size_t getvBufferSize(int index){return vboSize[index];};
        size_t getiBufferSize(int index){return iboSize[index];};
        uint16_t getiBufferType(int index){return iboTypeSize[index];};
        uint64_t getiBufferStride(int index){return iboStride[index];};
        unsigned int getTextureID(int index);
        size_t getPrimitiveSize(){return primitiveSize;}
        glm::vec4 getPBRColor(int j){return PBRColor[j];};
    private:
        std::vector<VertexBuffer> vbo;
        std::vector<IndexBuffer> ibo;
        std::vector<unsigned int> TextureID;
        std::vector<glm::vec4> PBRColor;
        std::vector<size_t> vboSize;
        std::vector<uint64_t> iboSize;
        std::vector<uint16_t> iboTypeSize;
        std::vector<uint64_t> iboStride;
        size_t primitiveSize = 0;
};

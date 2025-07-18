#pragma once
#ifndef GLV_PRMT_HPP
#define GLV_PRMT_HPP

#include <fx/gltf.h>
#include "general/GLVBuffer/VertexBuffer.hpp"
#include "general/GLVBuffer/IndexBuffer.hpp"
#include "general/GLVVertexArray/VertexArray.hpp"
#include "GLVElement.hpp"
#include "GLVMaterial.hpp"
#include "general/GLVFileLoad/absFile.hpp"
#include "MeshData.h"
#include "MaterialData.h"

#include <memory>

//End of Structure
namespace GLVGLTF{
    class GLVPrimitive : public GLVElement
    {
        private:
            int mat_index;
        public:
            struct BufferInfo{
                bool HasData = false;

                std::size_t BufferSize{};
                std::size_t GLCompType{};
                std::size_t DataStride{};
                std::size_t DataSize{};
                std::size_t elementSize{};

                std::size_t offset{};
            };

            enum BufferLocation{
                VERTEX_BUFFER,
                UV_BUFFER,
                NORMAL_BUFFER,
                TANGENT_BUFFER
            };

            VertexArray * vao = nullptr;
            VertexBuffer * vbo = nullptr;
            IndexBuffer * ibo = nullptr;
            VertexBuffer *vbo_t = nullptr; //VertexBuffer for Tangent

            //VBO Info
            std::size_t vboSize{};//totalBufferSize

            BufferInfo vBufferInfo{};
            BufferInfo nBufferInfo{};
            BufferInfo tBufferInfo{};
            BufferInfo cBufferInfo{};

            //IBO Info
            BufferInfo iBufferInfo{};

            //Material
            GLVMaterial * material;

            GLVPrimitive(absFile * absfile, std::size_t meshIndex, std::size_t primIndex);
            ~GLVPrimitive();

            void addProgram(unsigned int programID);
            void addLayout();

            void Bind();

            glm::vec3 compute(glm::vec3 pos1, glm::vec3 pos2,glm::vec3 pos3, glm::vec2 uv1,glm::vec2 uv2,glm::vec2 uv3);
            void CreateTangent(MeshData::BufferInfo const & vBuf, MeshData::BufferInfo const & nBuf, MeshData::BufferInfo const & cBuf, MeshData::BufferInfo const & iBuf);

            bool hasMaterial();
            void setMaterial(GLVMaterial* pMat);
            GLVMaterial* getMaterial(){return material;}
    };
}



#endif
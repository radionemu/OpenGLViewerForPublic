#include <fx/gltf.h>
#include "GLVMesh.h"
#include "MeshData.h"
#include "MaterialData.h"
#include "general/texture.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <nlohmann/json.hpp>

#include <iostream>

#define TEXNULL 4294967295

//inlcude if there is excessed data out of float size
size_t AlignedSize(size_t original_size){
    if(original_size % 4 == 0)return original_size;
    return original_size + (4 - (original_size % 4));
}

int GLVMesh::mimeType(std::string str)
{
    int strsize = str.size();
    if(str.substr(strsize-3, 4) == "png") return 0;
    if(str.substr(strsize-3, 4) == "jpg") return 1;
    if(str.substr(strsize-4, 5) == "jpeg") return 1;
    if(str.substr(strsize-3, 4) == "DDS") return 2;
    else return -1;
}

bool GLVMesh::Create(fx::gltf::Document const &doc, std::size_t meshIndex, std::vector<VertexArray> *vao, unsigned int programID, std::filesystem::path rootpath)
{
    //std::cout << doc.meshes[meshIndex].name << std::endl;
    
    //PRIMITIVES
    primitiveSize = doc.meshes[meshIndex].primitives.size();
    for(size_t i=0; i<primitiveSize; i++){
        printf("attribsize : %d\n",doc.meshes[meshIndex].primitives[i].attributes.size());
        MeshData mesh(doc, meshIndex, i); //document, mesh Index, primitive Index
        MeshData::BufferInfo const & vBuffer = mesh.VertexBuffer();
        MeshData::BufferInfo const & nBuffer = mesh.NormalBuffer();
        MeshData::BufferInfo const & tBuffer = mesh.TangentBuffer();
        MeshData::BufferInfo const & cBuffer = mesh.TexCoord0Buffer();
        MeshData::BufferInfo const & iBuffer = mesh.IndexBuffer();

        BufferLayout vLayout = BufferLayout();
        BufferLayout nLayout = BufferLayout();
        BufferLayout tLayout = BufferLayout();
        BufferLayout cLayout = BufferLayout();
        BufferLayout iLayout = BufferLayout();

        if(!vBuffer.HasData() || !iBuffer.HasData()){
            printf("CONTINUE\n");
            continue;
        }

        int SIZE_OF_VERTEX = 12;
        int SIZE_OF_NORMAL = 12;
        int SIZE_OF_TANGENT = 12;
        int SIZE_OF_UV = 8;

        size_t vBufferSIZE = static_cast<std::size_t>(vBuffer.TotalSize);
        size_t nBufferSIZE = static_cast<std::size_t>(nBuffer.TotalSize);
        size_t iBufferSIZE = static_cast<std::size_t>(iBuffer.TotalSize);
        size_t tBufferSIZE = static_cast<std::size_t>(tBuffer.TotalSize);
        size_t cBufferSIZE = static_cast<std::size_t>(cBuffer.TotalSize);
        size_t totalBuffersize = vBufferSIZE + nBufferSIZE + tBufferSIZE + cBufferSIZE;//get total buffer size
        //now malloc

        uint8_t *bufferStart = (uint8_t *)malloc(sizeof(uint8_t)*totalBuffersize);
        uint8_t *indexStart = (uint8_t *)malloc(sizeof(uint8_t)*iBufferSIZE);
        size_t offset = 0;
        vao->push_back(VertexArray());
        vao->back().Bind();
        vbo.push_back(VertexBuffer());
        vbo.back().Bind();
        ibo.push_back(IndexBuffer());
        ibo.back().Bind();

        //vBuffer
        std::memcpy(bufferStart, vBuffer.Data, vBufferSIZE);
        if(vBuffer.HasData()){
            vLayout.Push(programID, "vtxPosition", GL_FLOAT, 3, 0);
            vao->back().AttribBuffer(vbo.back(), vLayout);            
        }
        offset += vBufferSIZE;
        //nBuffer
        std::memcpy(bufferStart+offset, nBuffer.Data, nBufferSIZE);
        if(nBuffer.HasData()){
            nLayout.Push(programID, "vtxNormal", GL_FLOAT, 3, offset);
            vao->back().AttribBuffer(vbo.back(), nLayout);
        } 
        offset += nBufferSIZE;
        //cBuffer
        std::memcpy(bufferStart+offset, cBuffer.Data, cBufferSIZE);
        if(cBuffer.HasData()){
            cLayout.Push(programID, "vtxUV", GL_FLOAT, 2, offset);
            vao->back().AttribBuffer(vbo.back(), cLayout);
        }
        offset += cBufferSIZE;
        //tBuffer
        std::memcpy(bufferStart+offset, tBuffer.Data, tBuffer.TotalSize);
        if(tBuffer.HasData()){
            tLayout.Push(programID, "vtxTangent", GL_FLOAT, 3, offset);
            vao->back().AttribBuffer(vbo.back(), tLayout);
        }
        offset += tBuffer.TotalSize;
        //BufferData
        vbo.back().FillData(&bufferStart[0], offset);

        //BufferStart Test
  



        //iBuffer
        std::memcpy(indexStart, iBuffer.Data, iBufferSIZE);
        ibo.back().FillData(&indexStart[0], iBufferSIZE);
        
        printf("ibotypesize : %d, ibostride : %d\n", iBuffer.DataType, iBuffer.DataStride);
        vboSize.push_back(offset);
        iboSize.push_back(iBufferSIZE);
        iboTypeSize.push_back(iBuffer.DataType);
        iboStride.push_back(iBuffer.DataStride);

        free(bufferStart);
        free(indexStart);

        //MATERIAL
        MaterialData const & materialData = mesh.Material();
        //only if there is Material
        if(materialData.HasData()){
            auto material = materialData.Data();
            auto pbrMetalRough = material.pbrMetallicRoughness;
            if(pbrMetalRough.baseColorTexture.empty()) {
                printf("only pbr Color\n");
                TextureID.push_back(TEXNULL);
                PBRColor.push_back(glm::vec4(pbrMetalRough.baseColorFactor[0],pbrMetalRough.baseColorFactor[1],pbrMetalRough.baseColorFactor[2],pbrMetalRough.baseColorFactor[3]));
                printf("pbr Color : %f %f %f %f\n",PBRColor.back()[0],PBRColor.back()[1],PBRColor.back()[2],PBRColor.back()[3]);
                continue;
            }
            int32_t TextureIndex = pbrMetalRough.baseColorTexture.index;

            //Goto Texture            
            int32_t ImageIndex = doc.textures[TextureIndex].source;
            //first check if Texture is not embedded
            if(!doc.images[ImageIndex].IsEmbeddedResource()&&!doc.images[ImageIndex].uri.empty()){
                printf("seperate\n");
                std::filesystem::path fullPath = fx::gltf::detail::CreateBufferUriPath(rootpath, doc.images[ImageIndex].uri);
                printf("full path : %s\n", fullPath.string().c_str());
                if(doc.images[ImageIndex].mimeType == "image/png" || mimeType(doc.images[ImageIndex].uri) == 0){
                    TextureID.push_back(LoadPNG(fullPath.string().c_str()));
                }
                else if(doc.images[ImageIndex].mimeType == "image/jpg" || mimeType(doc.images[ImageIndex].uri)== 1){
                    TextureID.push_back(LoadJPG(fullPath.string().c_str()));
                }
                else if(doc.images[ImageIndex].mimeType == "image/vnd-ms.dds" || doc.images[ImageIndex].mimeType == "image/dds" || mimeType(doc.images[ImageIndex].uri)== 2){
                    TextureID.push_back(LoadDDS(fullPath.string().c_str()));
                }
            }
            else{
                printf("Embedded\n");
                if(doc.images[ImageIndex].IsEmbeddedResource()){
                    std::vector<uint8_t> embeddedData{};
                    doc.images[ImageIndex].MaterializeData(embeddedData);
                    uint32_t BinarySize = static_cast<uint32_t>(embeddedData.size());
                    uint8_t const * BinaryData = &embeddedData[0];
                }else{
                    fx::gltf::BufferView const & bufferView = doc.bufferViews[doc.images[ImageIndex].bufferView];
                    fx::gltf::Buffer const & buffer = doc.buffers[bufferView.buffer];

                    uint32_t BinarySize = bufferView.byteLength;
                    uint8_t const * BinaryData = &buffer.data[bufferView.byteOffset];

                    TextureID.push_back(LoadPNG_Mem(BinaryData, BinarySize));
                }
            }
        }
    }

    return false;
}

unsigned int GLVMesh::getTextureID(int index)
{
    return TextureID[index];
}

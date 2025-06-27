#include "GLVPrimitive.hpp"
#include "GLVMaterial.hpp"
#include "MeshData.h"
#include "MaterialData.h"
#include "general/GLVvertexArray/BufferLayout.hpp"

//Copy the Buffer to VBO & IBO
namespace GLVGLTF{

    GLVPrimitive::GLVPrimitive(absFile * absfile, std::size_t meshIndex, std::size_t primIndex)
    {
        this->DH = (fx::gltf::Document *)absfile->getDocument();
        this->element_name = "Primitive "+std::to_string(absfile->getPrimitiveNum());

        MeshData mesh(*DH, meshIndex, primIndex);
        MeshData::BufferInfo const & vBuffer = mesh.VertexBuffer();
        MeshData::BufferInfo const & nBuffer = mesh.NormalBuffer();
        MeshData::BufferInfo const & tBuffer = mesh.TangentBuffer();
        MeshData::BufferInfo const & cBuffer = mesh.TexCoord0Buffer();
        MeshData::BufferInfo const & iBuffer = mesh.IndexBuffer();

        if(!vBuffer.HasData()){
            std::cerr <<"Primitive ["<<primIndex<<"] does not have vBuffer\n";
            return;
        }

        vBufferInfo.HasData = vBuffer.HasData();
        nBufferInfo.HasData = nBuffer.HasData();
        tBufferInfo.HasData = tBuffer.HasData();
        cBufferInfo.HasData = cBuffer.HasData();

        size_t vBufferSIZE = vBufferInfo.BufferSize = static_cast<std::size_t>(vBuffer.TotalSize);
        size_t nBufferSIZE = nBufferInfo.BufferSize = static_cast<std::size_t>(nBuffer.TotalSize);
        size_t tBufferSIZE = tBufferInfo.BufferSize = static_cast<std::size_t>(tBuffer.TotalSize);
        size_t cBufferSIZE = cBufferInfo.BufferSize = static_cast<std::size_t>(cBuffer.TotalSize);
        size_t totalBuffersize = vBufferSIZE + nBufferSIZE + tBufferSIZE + cBufferSIZE;//get total buffer size

        uint8_t *bufferStart = (uint8_t *)malloc(sizeof(uint8_t)*totalBuffersize);
        size_t offset = 0;

        this->vao = new VertexArray();
        this->vbo = new VertexBuffer();
        this->ibo = new IndexBuffer();

        //Layout
        this->vBufferInfo.DataStride=vBuffer.DataStride;
        this->vBufferInfo.GLCompType=vBuffer.DataType;
        this->vBufferInfo.DataSize=vBuffer.elementSize;
        this->nBufferInfo.DataStride=nBuffer.DataStride;
        this->nBufferInfo.GLCompType=nBuffer.DataType;
        this->nBufferInfo.DataSize=nBuffer.elementSize;
        this->cBufferInfo.DataStride=cBuffer.DataStride;
        this->cBufferInfo.GLCompType=cBuffer.DataType;
        this->cBufferInfo.DataSize=cBuffer.elementSize;
        this->tBufferInfo.DataStride=tBuffer.DataStride;
        this->tBufferInfo.GLCompType=tBuffer.DataType;
        this->tBufferInfo.DataSize=tBuffer.elementSize;
        this->iBufferInfo.DataStride=iBuffer.DataStride;
        this->iBufferInfo.GLCompType=iBuffer.DataType;
        this->iBufferInfo.DataSize=iBuffer.elementSize;

        //vBuffer
        std::memcpy(bufferStart, vBuffer.Data, vBufferSIZE);
        vBufferInfo.offset = offset;
        offset += vBufferSIZE;
        //nBuffer
        std::memcpy(bufferStart+offset, nBuffer.Data, nBufferSIZE);
        nBufferInfo.offset = offset;
        offset += nBufferSIZE;
        //cBuffer
        std::memcpy(bufferStart+offset, cBuffer.Data, cBufferSIZE);
        cBufferInfo.offset = offset;
        offset += cBufferSIZE;
        //tBuffer
        std::memcpy(bufferStart+offset, tBuffer.Data, tBufferSIZE);
        tBufferInfo.offset = offset;
        offset += tBufferSIZE;
        //BufferData
        this->vbo->FillData(&bufferStart[0], offset);

        //BufferLayout
        addLayout();

        //iBuffer
        if(iBufferInfo.HasData = iBuffer.HasData()){
            size_t iBufferSIZE = iBufferInfo.BufferSize = static_cast<std::size_t>(iBuffer.TotalSize);
            this->ibo->FillData(iBuffer.Data, iBufferSIZE);
            this->iBufferInfo.elementSize=this->iBufferInfo.BufferSize/this->iBufferInfo.DataStride;
        }

        //new Tangent Buffer
        if(!tBuffer.HasData() && cBuffer.HasData() && iBuffer.HasData() && vBuffer.HasData()){
            CreateTangent(vBuffer,nBuffer,cBuffer, iBuffer);
        }

        //Material
        auto matIndex = DH->meshes[meshIndex].primitives[primIndex].material;
        this->mat_index = matIndex;
        if(matIndex >= 0){
            //find material from global list
            std::vector<GLVMaterial *>* globalMatList = reinterpret_cast<std::vector<GLVMaterial *>*>(absfile->getMaterialList());
            int lIndex = -1;
            for(GLVMaterial* lMat : *globalMatList){
                lIndex = lMat->getMatIndex();
                printf("getMattIndex [%d]\n", lIndex);
                if(lIndex == matIndex){
                    printf("Share the Material // MATINDEX[%d]\n", matIndex);
                    this->material = (lMat);
                    break;
                }
                lIndex = -1;
            }
            if(lIndex == -1){
                printf("Create New Material // MATINDEX[%d]\n", matIndex);
                this->material = new GLVMaterial(absfile, matIndex);
                globalMatList->emplace_back(material);
            }
        }
    }

    glm::vec3 GLVPrimitive::compute(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
    {
        glm::vec3 edge1 = pos2-pos1; //A0
        glm::vec3 edge2 = pos3-pos1; //A1
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        glm::mat2 A = glm::mat2(deltaUV1.x, deltaUV2.x, deltaUV1.y, deltaUV2.y);
        glm::mat2 Ai = glm::inverse(A);

        // float det = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 ltangent = glm::vec3(0.0f);
            // float invDet = 1.0f / det;
            // float lx = (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            // float ly = (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            // float lz = (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            // ltangent.x = invDet * lx/2.0f;
            // ltangent.y = invDet * ly/2.0f;
            // ltangent.z = invDet * lz/2.0f;
        ltangent = glm::vec3(Ai[0].x * edge1.x + Ai[0].y* edge2.x, Ai[0].x * edge1.y + Ai[0].y* edge2.y, Ai[0].x * edge1.z + Ai[0].y* edge2.z);
        ltangent = glm::normalize(ltangent);
        // if(std::isnan(ltangent.x)){
        //     printf("tangent %f %f %f\n", ltangent.x, ltangent.y, ltangent.z);
        // }        
        return ltangent;
    }

    //do not touch
    void GLVPrimitive::CreateTangent(MeshData::BufferInfo const &vBuf, MeshData::BufferInfo const &nBuf, MeshData::BufferInfo const &cBuf, MeshData::BufferInfo const &iBuf)
    {
        printf("Create New Tangent");
        this->vbo_t = new VertexBuffer();
        vbo_t->Bind();
        uint8_t* tBufferData = (uint8_t *)malloc(sizeof(uint8_t)*static_cast<std::size_t>(vBuf.TotalSize));
        for(size_t i = 0; i<iBufferInfo.BufferSize; i+=(3*iBuf.DataStride)){
            size_t ind[3];
            //copy index data
            if(iBufferInfo.GLCompType == (uint16_t)fx::gltf::Accessor::ComponentType::UnsignedByte){
                ind[0] = ((unsigned char)(iBuf.Data[i]));
                ind[1] = ((unsigned char)(iBuf.Data[i+1]));
                ind[2] = ((unsigned char)(iBuf.Data[i+2]));
            }else if(iBufferInfo.GLCompType == (uint16_t)fx::gltf::Accessor::ComponentType::UnsignedShort){
                ind[0] = *((unsigned short*)(iBuf.Data+i));
                ind[1] = *((unsigned short*)(iBuf.Data+i+2));
                ind[2] = *((unsigned short*)(iBuf.Data+i+4));
            }else if(iBufferInfo.GLCompType == (uint16_t)fx::gltf::Accessor::ComponentType::UnsignedInt){
                ind[0] = (unsigned int)(iBuf.Data[i]);
                ind[1] = (unsigned int)(iBuf.Data[i+4]);
                ind[2] = (unsigned int)(iBuf.Data[i+8]);
            }
            //vertex is float type (I guess...?)
            glm::vec3 pos1 = glm::vec3(*(float *)(vBuf.Data+ind[0]*12),*(float *)(vBuf.Data+ind[0]*12+4),*(float *)(vBuf.Data+ind[0]*12+8));
            glm::vec3 pos2 = glm::vec3(*(float *)(vBuf.Data+ind[1]*12),*(float *)(vBuf.Data+ind[1]*12+4),*(float *)(vBuf.Data+ind[1]*12+8));
            glm::vec3 pos3 = glm::vec3(*(float *)(vBuf.Data+ind[2]*12),*(float *)(vBuf.Data+ind[2]*12+4),*(float *)(vBuf.Data+ind[2]*12+8));
            glm::vec2 uv1 = glm::vec2(*(float *)(cBuf.Data+ind[0]*8),*(float *)(cBuf.Data+ind[0]*8+4));
            glm::vec2 uv2 = glm::vec2(*(float *)(cBuf.Data+ind[1]*8),*(float *)(cBuf.Data+ind[1]*8+4));
            glm::vec2 uv3 = glm::vec2(*(float *)(cBuf.Data+ind[2]*8),*(float *)(cBuf.Data+ind[2]*8+4));

            glm::vec3 tan = compute(pos1,pos2,pos3,uv1,uv2,uv3);

            *(float *)(tBufferData+ind[0]*12) = (float)tan.x; *(float *)(tBufferData+ind[0]*12+4) = (float)tan.y; *(float *)(tBufferData+ind[0]*12+8) = (float)tan.z;
            *(float *)(tBufferData+ind[1]*12) = (float)tan.x; *(float *)(tBufferData+ind[1]*12+4) = (float)tan.y; *(float *)(tBufferData+ind[1]*12+8) = (float)tan.z;
            *(float *)(tBufferData+ind[2]*12) = (float)tan.x; *(float *)(tBufferData+ind[2]*12+4) = (float)tan.y; *(float *)(tBufferData+ind[2]*12+8) = (float)tan.z;
        }

        this->vbo_t->FillData(&(tBufferData)[0], sizeof(uint8_t)*vBuf.TotalSize);
        BufferLayout tLayout = BufferLayout();
        tLayout.DirectPush(TANGENT_BUFFER, vBufferInfo.GLCompType, vBufferInfo.DataStride/vBufferInfo.DataSize, 0);
        this->vao->AttribBuffer(*(this->vbo_t), tLayout);
    }

    GLVPrimitive::~GLVPrimitive()
    {
    }

    void GLVPrimitive::addProgram(unsigned int programID)
    {
    }

    void GLVPrimitive::addLayout()
    {
        BufferLayout vLayout = BufferLayout();
        BufferLayout nLayout = BufferLayout();
        BufferLayout tLayout = BufferLayout();
        BufferLayout cLayout = BufferLayout();
        BufferLayout iLayout = BufferLayout();

        this->vao->Bind(); // Bind VertexArray
        if(vBufferInfo.HasData){
            vLayout.DirectPush(VERTEX_BUFFER, vBufferInfo.GLCompType, vBufferInfo.DataStride/vBufferInfo.DataSize, vBufferInfo.offset);
            this->vao->AttribBuffer(*(this->vbo), vLayout);
            printf("vL ADDED\n");
        }
        if(nBufferInfo.HasData){
            nLayout.DirectPush(NORMAL_BUFFER, nBufferInfo.GLCompType, nBufferInfo.DataStride/nBufferInfo.DataSize, nBufferInfo.offset);
            this->vao->AttribBuffer(*(this->vbo), nLayout);
            printf("nL ADDED\n");
        }
        if(cBufferInfo.HasData){
            cLayout.DirectPush(UV_BUFFER, cBufferInfo.GLCompType, cBufferInfo.DataStride/cBufferInfo.DataSize, cBufferInfo.offset);
            this->vao->AttribBuffer(*(this->vbo), cLayout);
            printf("cL ADDED\n");
        }
        if(tBufferInfo.HasData){
            tLayout.DirectPush(TANGENT_BUFFER, tBufferInfo.GLCompType, tBufferInfo.DataStride/tBufferInfo.DataSize, tBufferInfo.offset);
            this->vao->AttribBuffer(*(this->vbo), tLayout);
            printf("tL ADDED\n");
        }
        printf("program ADDED\n");
    }

    void GLVPrimitive::Bind()
    {
        this->vao->Bind();
    }

    bool GLVPrimitive::hasMaterial()
    {
        return mat_index>=0;
    }
    
    void GLVPrimitive::setMaterial(GLVMaterial* pMat)
    {
        // (this->material) = std::shared_ptr<GLVMaterial>(pMat);
        this->material = pMat;

        mat_index = pMat->getMatIndex();
    }
}

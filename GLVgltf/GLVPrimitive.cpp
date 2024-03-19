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

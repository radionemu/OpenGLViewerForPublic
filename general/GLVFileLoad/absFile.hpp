#pragma once

namespace GLVGLTF{
    class absFile
    {
    private:
        /* data */
    public:
        virtual void addrtNodes(void* data)=0;
        virtual void addMeshes(void* data)=0;
        virtual void addPrimitives(void* data)=0;
        virtual void addTextures(void* data)=0;

        virtual void* getDocument()=0;
        virtual void* getModelPath()=0;
        virtual void* getMaterialList()=0;

        virtual size_t getSceneNum()=0;
        virtual size_t getNodeNum()=0;
        virtual size_t getMeshNum()=0;
        virtual size_t getPrimitiveNum()=0;
        virtual size_t getMaterialNum()=0;
        virtual size_t getTextureNum()=0;

    };
    
}
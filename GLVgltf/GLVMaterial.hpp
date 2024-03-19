#pragma once
#ifndef GLV_MAT_HPP
#define GLV_MAT_HPP

#include "MaterialData.h"
#include "general/GLVBuffer/UniformBuffer.hpp"
#include "GLVElement.hpp"
#include "GLVTexture.hpp"
#include <glm/glm.hpp>
#include <fx/gltf.h>
#include "general/GLVFileLoad/absFile.hpp"

namespace GLVGLTF{
    class GLVMaterial : public GLVElement
    {
    private:
        std::size_t matIndex;//material ID
        fx::gltf::Material material;

        //pbrMetallicRoughness
            GLVTexture * baseColorTexture = nullptr;
                GLVTexture * BaseColorTexture_Tmp =nullptr;
            std::int64_t baseClrTexIndex{-1};
            std::int64_t baseClrTexID{-1};
            //TEXCOORD!!
            glm::vec4 baseColorFactor = glm::vec4(0.0f);

            GLVTexture * metallicRoughnessTexture = nullptr;
                GLVTexture * metallicRoughnessTexture_Tmp = nullptr;
            std::int64_t metalRoughTexIndex{-1};
            std::int64_t metalRoughTexID{-1};
            //TEXCOORD!!
            float metallicFactor{0.0f};
            float RoughnessFactor{0.0f};
        
        //normalTexture
            float scale{0.0f};
            GLVTexture * normalTexture = nullptr;
                GLVTexture * normalTexture_Tmp = nullptr;
            std::int64_t normalTexIndex{-1};
            std::int64_t normalTexID{-1};
            //TEXCOORD!!

        //OcclusionTexture
            float strength{0.0f};
            GLVTexture * occlusionTexture = nullptr;
                GLVTexture * occlusionTexture_Tmp = nullptr;
            std::int64_t occlusionTexIndex{-1};
            std::int64_t occlusionTexID{-1};
            //TEXCOORD!!

        //Emmisive Factor
            GLVTexture * emissiveTexture = nullptr;
                GLVTexture * emissiveTexture_Tmp = nullptr;
            std::int64_t emissiveTexIndex{-1};
            std::int64_t emissiveTexID{-1};
            //TEXCOORD!!
            glm::vec4 emissiveFactor = glm::vec4(0.0f);

    public:
        GLVMaterial(absFile * absfile, size_t matIndex);
        ~GLVMaterial();

        GLuint CreateMaterialData(fx::gltf::Document const & doc, std::filesystem::path modelPath);
        
        //pbrMetallicRoughness
        bool isBaseClrUVEmpty(){return baseColorTexture == nullptr;}
        unsigned int getBaseClrUV(){return baseColorTexture->GetGLVtextureID();}
        glm::vec4* getBaseClrFactor(){return &baseColorFactor;}

        bool isMetalRoughUVEmpty(){return metallicRoughnessTexture == nullptr;}
        unsigned int getMetRoughUV(){return metallicRoughnessTexture->GetGLVtextureID();}
        float* getMetallic(){return &metallicFactor;}
        float* getRoughness(){return &RoughnessFactor;}

        //normalTexture
        bool isNormalUVEmpty(){return normalTexture == nullptr;}
        unsigned int getNormalUV(){return normalTexture->GetGLVtextureID();}
        float* getNormalScale(){return &scale;}

        //OcclusionTexture
        bool isOcclusionUVEmpty(){return occlusionTexture == nullptr;}
        unsigned int getOcclusionUV(){return occlusionTexture->GetGLVtextureID();}
        float* getOcclusionStren(){return &strength;}

        //EmissiveTexture
        bool isEmissiveUVEmpty(){return emissiveTexture == nullptr;}
        unsigned int getEmissiveUV(){return emissiveTexture->GetGLVtextureID();}
        glm::vec4* getEmissiveFac(){return &emissiveFactor;}

        size_t getMatIndex(){return matIndex;}
    
        //for Inspector View
        bool isBaseClrTmpEmpty(){return BaseColorTexture_Tmp == nullptr;}
        bool isMetalRoughTmpEmpty(){return metallicRoughnessTexture_Tmp == nullptr;}
        bool isNormalTmpEmpty(){return normalTexture_Tmp == nullptr;}
        bool isOcclusionTmpEmpty(){return occlusionTexture_Tmp == nullptr;}
        bool isEmissiveTmpEmpty(){return emissiveTexture_Tmp == nullptr;}

        void setBaseClrTmp(GLVTexture * srcTmp){BaseColorTexture_Tmp = srcTmp;}
        void setMetalRoughTmp(GLVTexture * srcTmp){metallicRoughnessTexture_Tmp = srcTmp;}
        void setNormalTmp(GLVTexture * srcTmp){normalTexture_Tmp = srcTmp;}
        void setOcclusionTmp(GLVTexture * srcTmp){occlusionTexture_Tmp = srcTmp;}
        void setEmissiveTmp(GLVTexture * srcTmp){emissiveTexture_Tmp = srcTmp;}

        unsigned int getBaseClrUVTmp(){return BaseColorTexture_Tmp->GetGLVtextureID();}
        unsigned int getMetalRoughUVTmp(){return metallicRoughnessTexture_Tmp->GetGLVtextureID();}
        unsigned int getNormalUVTmp(){return normalTexture_Tmp->GetGLVtextureID();}
        unsigned int getOcclusionUVTmp(){return occlusionTexture_Tmp->GetGLVtextureID();}
        unsigned int getEmissiveUVTmp(){return emissiveTexture_Tmp->GetGLVtextureID();}
        
        void RevertTmp();
        void ApplyTmp();
    };
}
#endif
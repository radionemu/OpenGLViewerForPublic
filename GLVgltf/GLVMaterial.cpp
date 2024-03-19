#include "GLVMaterial.hpp"
#include "GLVTexture.hpp"
#include <fx/gltf.h>

namespace GLVGLTF{

    GLVMaterial::GLVMaterial(absFile *absfile, std::size_t matIndex)
    {
        this->DH = (fx::gltf::Document *)absfile->getDocument();
        this->matIndex = matIndex;
        this->material = DH->materials[matIndex];
        this->element_name = "Material "+std::to_string(matIndex);

        auto & pbrMetalRough = this->material.pbrMetallicRoughness;
        auto & normTexture = this->material.normalTexture;
        auto & occTexture = this->material.occlusionTexture;
        auto & emiTexture = this->material.emissiveTexture;
        auto & emiFactor = this->material.emissiveFactor;

        //pbrMetallicRoughness
        if(!pbrMetalRough.empty()){
            auto & bColTex = pbrMetalRough.baseColorTexture;
            auto & bColFac = pbrMetalRough.baseColorFactor;
            auto & mRoughTex = pbrMetalRough.metallicRoughnessTexture;
            auto & metalFac = pbrMetalRough.metallicFactor;
            auto & RoughFac = pbrMetalRough.roughnessFactor;

            //baseColor Texture
            if(!bColTex.empty()){
                this->baseClrTexIndex = bColTex.index;
                this->baseColorTexture = new GLVTexture(absfile, baseClrTexIndex);
                absfile->addTextures(baseColorTexture);
                this->baseClrTexID = baseColorTexture->GetGLVtextureID();
            }
            //baseColorFactor
            else{ // must recheck
                std::memcpy(&baseColorFactor[0], &bColFac[0], sizeof(bColFac));
                // this->baseColorFactor = glm::vec4(bColFac[0],bColFac[1],bColFac[2],bColFac[3]);
            }
            //metallicRoughness Texture
            if(!mRoughTex.empty()){
                this->metalRoughTexIndex = mRoughTex.index;
                this->metallicRoughnessTexture = new GLVTexture(absfile, metalRoughTexIndex);
                absfile->addTextures(metallicRoughnessTexture);
                this->metalRoughTexID = metallicRoughnessTexture->GetGLVtextureID();
            }
            this->metallicFactor = metalFac;
            this->RoughnessFactor = RoughFac;
        }
        //normalTexture
        if(!normTexture.empty()){
            this->scale = normTexture.scale;
            this->normalTexIndex = normTexture.index;
            this->normalTexture = new GLVTexture(absfile, normalTexIndex);
            absfile->addTextures(normalTexture);
            this->normalTexID = normalTexture->GetGLVtextureID();
        }

        //occlusionTexture
        if(!occTexture.empty()){
            this->strength = occTexture.strength;
            this->occlusionTexIndex = occTexture.index;
            this->occlusionTexture = new GLVTexture(absfile, occlusionTexIndex);
            absfile->addTextures(occlusionTexture);
            this->occlusionTexID = occlusionTexture->GetGLVtextureID();
        }

        //emissiveTexture
        if(!emiTexture.empty()){
            this->emissiveTexIndex = emiTexture.index;
            this->emissiveTexture = new GLVTexture(absfile, emissiveTexIndex);
            absfile->addTextures(emissiveTexture);
            this->emissiveTexID = emissiveTexture->GetGLVtextureID();
        }
        //emissiveFactor
        if(!emiFactor.empty()){
            std::memcpy(&emissiveFactor[0], &emiFactor[0], sizeof(emiFactor));
        }
    }

    GLVMaterial::~GLVMaterial()
    {
    }
    
    void GLVMaterial::RevertTmp()
    {
        if(!isBaseClrTmpEmpty()&&BaseColorTexture_Tmp->isExtern())delete BaseColorTexture_Tmp;
        BaseColorTexture_Tmp = nullptr;
        if(!isMetalRoughTmpEmpty() && metallicRoughnessTexture_Tmp->isExtern())delete metallicRoughnessTexture_Tmp;
        metallicRoughnessTexture_Tmp = nullptr;
        if(!isNormalTmpEmpty() && normalTexture_Tmp->isExtern())delete normalTexture_Tmp;
        normalTexture_Tmp = nullptr;
        if(!isOcclusionTmpEmpty() && occlusionTexture_Tmp->isExtern())delete occlusionTexture_Tmp;
        occlusionTexture_Tmp = nullptr;
        if(!isEmissiveTmpEmpty() && emissiveTexture_Tmp->isExtern())delete emissiveTexture_Tmp;
        emissiveTexture_Tmp = nullptr;
    }

    void GLVMaterial::ApplyTmp()
    {
        if(!isBaseClrTmpEmpty())baseColorTexture = BaseColorTexture_Tmp;
        if(!isMetalRoughTmpEmpty())metallicRoughnessTexture = metallicRoughnessTexture_Tmp;
        if(!isNormalTmpEmpty())normalTexture = normalTexture_Tmp;
        if(!isOcclusionTmpEmpty())occlusionTexture = occlusionTexture_Tmp;
        if(!isEmissiveTmpEmpty())emissiveTexture = emissiveTexture_Tmp;

        BaseColorTexture_Tmp = nullptr;
        metallicRoughnessTexture_Tmp = nullptr;
        normalTexture_Tmp = nullptr;
        occlusionTexture_Tmp = nullptr;
        emissiveTexture_Tmp = nullptr;
    }
}

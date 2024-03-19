#pragma once
#ifndef GLVFILE_HPP
#define GLVFILE_HPP
#include <filesystem>
#include <fx/gltf.h>
#include "absFile.hpp"
#include "GLVgltf/GLVScene.hpp"
#include "GLVgltf/GLVPrimitive.hpp"
#include "GLVgltf/GLVMaterial.hpp"
#include "GLVgltf/GLVNode.hpp"
#include "GLVgltf/GLVMeshes.hpp"
#include <queue>

namespace fs = std::filesystem;

namespace GLVGLTF{
    class GLVFile : public absFile
    {
    private:
        fs::path * filePath=nullptr;
        size_t scene_count = 0;
        size_t node_count = 0;
        size_t mesh_count = 0;
        size_t prim_count = 0;
        size_t mat_count = 0;

    public:
        fx::gltf::Document* DH = nullptr;
        std::vector<GLVScene *> scenes; //collect all Scenes
        std::vector<GLVNode *> rtNodes; //collect rootNodes
        std::vector<GLVMeshes *> Meshes;
        std::vector<GLVPrimitive *> Primitives; //collect All Primitives
        std::vector<GLVMaterial *> Materials;
        std::vector<GLVTexture *> Textures;
        glm::vec3 mMax = glm::vec3(-9999999);
        glm::vec3 mMin = glm::vec3(9999999);

        GLVFile(fs::path filePath);
        ~GLVFile();

        void addrtNodes(void* data) override;
        void addMeshes(void *data) override;
        void addPrimitives(void* data) override;
        void addTextures(void* data) override;

        void* getDocument() override;
        void* getModelPath() override;
        void* getMaterialList() override;

        virtual size_t getSceneNum() override;
        virtual size_t getNodeNum() override;
        virtual size_t getMeshNum() override;
        virtual size_t getPrimitiveNum() override;
        virtual size_t getMaterialNum() override;
        virtual size_t getTextureNum() override;

        void addProgram(unsigned int programID);
        void updateMinMax();

        glm::vec3 getmMax(){return mMax;}
        glm::vec3 getmMin(){return mMin;}

        std::string getFileName();
        std::vector<GLVScene *>* getScenes(){return &scenes;};
        std::vector<GLVPrimitive *>* getPrimitives(){return &Primitives;};
        std::vector<GLVTexture *>* getTextures(){return &Textures;}
    };
}



#endif
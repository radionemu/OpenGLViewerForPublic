#include "GLVFile.hpp"
#include <filesystem>

namespace fs = std::filesystem;

//Managing DH : all information is in DH

namespace GLVGLTF{
    GLVFile::GLVFile(fs::path filePath)
    {
        (this->filePath) = new fs::path(filePath);
        this->DH = new fx::gltf::Document();
        //Create Document
        if(filePath.extension() == ".gltf"){
            *(this->DH) = fx::gltf::LoadFromText(filePath);
        }else if(filePath.extension() == ".glb"){
            *(this->DH) = fx::gltf::LoadFromBinary(filePath);
        }
        for(auto scene : DH->scenes){
            GLVScene * nScene = new GLVScene(this, scene);
            this->scenes.emplace_back(nScene);
        }
        //minmax
        updateMinMax();
    }

    GLVFile::~GLVFile()
    {
    }

    //override
    void GLVFile::addrtNodes(void *data)
    {
        this->rtNodes.emplace_back(reinterpret_cast<GLVNode *>(data));
    }

    void GLVFile::addMeshes(void *data)
    {
        this->Meshes.emplace_back(reinterpret_cast<GLVMeshes *>(data));
    }

    void GLVFile::addPrimitives(void *data)
    {
        this->Primitives.emplace_back(reinterpret_cast<GLVPrimitive *>(data));
    }

    void GLVFile::addTextures(void *data)
    {
        this->Textures.emplace_back(reinterpret_cast<GLVTexture *>(data));
    }

    void *GLVFile::getDocument()
    {
        return this->DH;
    }

    void *GLVFile::getModelPath()
    {
        return this->filePath;
    }

    void *GLVFile::getMaterialList()
    {
        return &(this->Materials);
    }

    size_t GLVFile::getSceneNum()
    {
        return scene_count++;
    }

    size_t GLVFile::getNodeNum()
    {
        return node_count++;
    }

    size_t GLVFile::getMeshNum()
    {
        return mesh_count;
    }

    size_t GLVFile::getPrimitiveNum()
    {
        return prim_count++;
    }

    size_t GLVFile::getMaterialNum()
    {
        return mat_count++;
    }

    void GLVFile::addProgram(unsigned int programID)
    {
        for(auto scene : this->scenes){
            scene->addProgram(programID);
        }
    }

    size_t GLVFile::getTextureNum()
    {
        return Textures.size();
    }

    void GLVFile::updateMinMax()
    {
        auto & accessors = DH->accessors;
        for(int i=0; i<accessors.size(); i++){
            if(!accessors[i].min.empty() && !accessors[i].max.empty() && accessors[i].type == fx::gltf::Accessor::Type::Vec3){
                mMax.x = accessors[i].max[0] > mMax.x ? accessors[i].max[0] : mMax.x;
                mMax.y = accessors[i].max[2] > mMax.y ? accessors[i].max[2] : mMax.y;
                mMax.z = accessors[i].max[1] > mMax.z ? accessors[i].max[1] : mMax.z;

                mMin.x = accessors[i].min[0] < mMin.x ? accessors[i].min[0] : mMin.x;
                mMin.y = accessors[i].min[2] < mMin.y ? accessors[i].min[2] : mMin.y;
                mMin.z = accessors[i].min[1] < mMin.z ? accessors[i].min[1] : mMin.z;
            }
        }
    }
    
    std::string GLVFile::getFileName()
    {
        return this->filePath->stem().string();
    }
}

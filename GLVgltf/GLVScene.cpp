#include "GLVScene.hpp"
#include <fx/gltf.h>

namespace GLVGLTF{
    GLVScene::GLVScene(absFile * absfile, fx::gltf::Scene scene)
    {
        this->DH = (fx::gltf::Document *)absfile->getDocument();
        this->scene = scene;

        if(!scene.name.empty()){
            this->element_name = scene.name;
        }else{
            this->element_name = "Scene "+ std::to_string(absfile->getSceneNum());
        }

        for(uint32_t rtNodeIndex : scene.nodes){
            GLVNode * rtnode = new GLVNode(absfile, rtNodeIndex);
            this->rtNodes.emplace_back(rtnode); //add rootNodes in this class
            absfile->addrtNodes(rtnode); //add rootNodes in GLVFile
        }
    }

    GLVScene::~GLVScene()
    {
    }

    void GLVScene::addProgram(unsigned int programID)
    {
        for(auto & rtnds : this->rtNodes){
            rtnds->addProgram(programID);
        }
    }
}

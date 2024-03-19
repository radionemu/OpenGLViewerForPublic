#pragma once
#ifndef GLV_SCENE_HPP
#define GLV_SCENE_HPP

#include <vector>
#include <fx/gltf.h>
#include "GLVNode.hpp"
#include "GLVElement.hpp"
#include "general/GLVFileLoad/absFile.hpp"

namespace GLVGLTF{
    class GLVScene : public GLVElement
    {
        private:
            //matrices for scene
            //This is because there can be multiple scenes on one GLTF files. In this application it renders all of elements.
            fx::gltf::Scene scene;
            
            std::vector<GLVNode *> rtNodes;

            glm::mat4 matrix = glm::mat4(1.0f);//Local transform
            glm::vec3 translation = glm::vec4(0.0f);
            glm::quat rotation = glm::quat(0.0f,0.0f,0.0f,0.0f);
            glm::vec3 scale = glm::vec4(1.0f);
        public:
            

            GLVScene(absFile * absfile, fx::gltf::Scene scene);
            ~GLVScene();
            
            size_t getrtNodeNum(){return this->rtNodes.size();}
            std::vector<GLVNode *>* getRTNode(){return &this->rtNodes;}

            void addProgram(unsigned int programID);
    };
}



#endif
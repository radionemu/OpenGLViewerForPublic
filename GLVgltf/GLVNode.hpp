#pragma once
#ifndef GLV_NODE_HPP
#define GLV_NODE_HPP

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <fx/gltf.h>

#include "GLVElement.hpp"
#include "GLVCamera.hpp"
#include "GLVMeshes.hpp"
#include "general/GLVFileLoad/absFile.hpp"

namespace GLVGLTF{
    class GLVNode : public GLVElement
    {
        private:
            //Identifier
            std::size_t identifier;

            //Children
            std::vector<GLVNode *> ChildNodes;

            //matrices
            glm::mat4 matrix = glm::mat4(1.0f);//Local transform
            glm::vec3 translation = glm::vec4(0.0f);
            glm::quat rotation = glm::quat(0.0f,0.0f,0.0f,0.0f);
            glm::vec3 scale = glm::vec4(0.0f);
            
            //Mesh
            GLVMeshes *mesh = NULL;
            
            //Camera

        public:
            GLVNode(absFile * absfile, std::size_t nodeIndex);
            ~GLVNode();

            bool hasChild();
            bool hasMesh();
            int Childrensize();
            void addProgram(unsigned int programID);

            std::vector<GLVNode *>* getChildNodes(){return &this->ChildNodes;}
            GLVMeshes * getMesh(){return this->mesh;}
    };
}


#endif
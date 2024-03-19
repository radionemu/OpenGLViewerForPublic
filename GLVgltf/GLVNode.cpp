#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fx/gltf.h>
#include "GLVNode.hpp"
#include "GLVMeshes.hpp"
#include "general/GLVFileLoad/absFile.hpp"

//TEST COMPLETE

namespace GLVGLTF{
    GLVNode::GLVNode(absFile * absfile, std::size_t nodeIndex)
    {
        this->DH = (fx::gltf::Document *)absfile->getDocument();
        //init
        this->identifier = nodeIndex;
        fx::gltf::Node const & node = DH->nodes[nodeIndex];

        if(!node.name.empty()){
            this->element_name = node.name;
        }else{
            this->element_name = "Node "+std::to_string(absfile->getNodeNum());
        }

        //Matrix -> Mesh -> Child Node 
        //calculate Matrix
        if(node.matrix.empty()){
            printf("CALCULATE MATRIX\n");
            if(!node.translation.empty()){
                this->translation = glm::vec3(node.translation[0],node.translation[1],node.translation[2]);
            }
            if(!node.rotation.empty()){
                this->rotation = glm::quat(node.rotation[0],node.rotation[1],node.rotation[2],node.rotation[3]);
            }
            if(!node.scale.empty()){
                this->scale = glm::vec3(node.scale[0],node.scale[1],node.scale[2]);
            }
            //assume all vectors have normal values
            glm::mat4 S = glm::scale(glm::mat4(1.0f), this->scale);
            glm::mat4 R = glm::toMat4(this->rotation);
            glm::mat4 T = glm::translate(glm::mat4(1.0f), this->translation);
            this->matrix = T * R * S;
        }else{
            printf("memcpy matrix\n");
            memcpy(glm::value_ptr(this->matrix), &(node.matrix)[0], sizeof(node.matrix));
        }

        //Mesh
        if(node.mesh != -1){
            printf("APPENDING MESH[%d]\n", node.mesh);
            this->mesh = new GLVMeshes(absfile, node.mesh);
            absfile->addMeshes(this->mesh);
        }

        //Camera
        if(node.camera != -1){

        }
        //add children
        if(!node.children.empty()){
            for(uint32_t chldIndex : node.children){
                try
                {
                   this->ChildNodes.emplace_back(new GLVNode(absfile, chldIndex));
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    std::cerr << "Adding child No."<<chldIndex<<"from Node ["<<this->identifier<<"] has failed." << '\n';
                }
            }
        }
    }

    GLVNode::~GLVNode()
    {
        // delete(this->mesh);
    }

    bool GLVNode::hasChild()
    {
        return this->ChildNodes.size() > 0;
    }

    bool GLVNode::hasMesh()
    {
        return this->mesh != NULL;
    }

    int GLVNode::Childrensize()
    {
        return this->ChildNodes.size();
    }

    void GLVNode::addProgram(unsigned int programID)
    {
        if(this->mesh != nullptr){
            this->mesh->addProgram(programID);
        }
        if(hasChild()){
            for(auto & nodes : this->ChildNodes){
                nodes->addProgram(programID);
            }
        }
    }
}

#include "GLVMeshes.hpp"
#include "GLVPrimitive.hpp"
#include "general/GLVFileLoad/absFile.hpp"

#include <fx/gltf.h>
#include <vector>

namespace GLVGLTF{

    GLVMeshes::GLVMeshes(absFile *absfile, std::size_t meshIndex)
    {
        this->DH = (fx::gltf::Document *)absfile->getDocument();
        this->meshIndex = meshIndex;
        auto const & mesh = DH->meshes[meshIndex];
        if(!mesh.name.empty()){
            this->element_name = mesh.name;
        }else{
            this->element_name = "Mesh "+std::to_string(absfile->getMeshNum());
        }

        for(size_t i=0; i<mesh.primitives.size(); i++){
            printf("PRIMITIVE [%d]\n", i);
            GLVPrimitive * nPrim = new GLVPrimitive(absfile, meshIndex, i);
            this->primitives.emplace_back(nPrim);
            absfile->addPrimitives(nPrim);
        }
    }

    GLVMeshes::~GLVMeshes()
    {
    }

    bool GLVMeshes::hasPrimitive()
    {
        return this->primitives.size()>0;
    }

    size_t GLVMeshes::sizePrimitive()
    {
        return this->primitives.size();
    }

    void GLVMeshes::addProgram(unsigned int programID)
    {
        for(auto & prim : this->primitives){
            prim->addProgram(programID);
        }
    }

}

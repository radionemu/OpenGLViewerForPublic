#pragma once
#ifndef GLV_MESH_HPP
#define GLV_MESH_HPP

#include <fx/gltf.h>
#include <vector>
#include "GLVElement.hpp"
#include "GLVPrimitive.hpp"
#include "general/GLVFileLoad/absFile.hpp"

namespace GLVGLTF{
    class GLVMeshes : public GLVElement
    {
        private:
            std::size_t meshIndex;
            std::vector<GLVPrimitive *> primitives;
        public:

            GLVMeshes(absFile *absfile, std::size_t meshIndex);
            ~GLVMeshes();

            bool hasPrimitive();
            size_t sizePrimitive();

            void addProgram(unsigned int programID);
            std::vector<GLVPrimitive *>* getPrimitives(){return &this->primitives;}
    };
}



#endif
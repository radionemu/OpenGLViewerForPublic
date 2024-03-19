#pragma once
#include <fx/gltf.h>

namespace GLVGLTF
{
    class GLVElement
    {
    protected:
        fx::gltf::Document* DH = nullptr;
        std::string element_name;

    public:
        GLVElement();
        ~GLVElement();
        virtual std::string getElementName(){return element_name;}
    };
    
} // namespace GLVGLTF

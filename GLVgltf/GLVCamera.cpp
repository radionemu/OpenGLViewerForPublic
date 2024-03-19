#include "GLVCamera.hpp"

#include <string>
#include <fx/gltf.h>


GLVCamera::GLVCamera(fx::gltf::Document const &doc, std::size_t camIndex)
{
    fx::gltf::Camera camera = doc.cameras[camIndex];
    
}
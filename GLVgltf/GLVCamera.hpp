#pragma once
#ifndef GLV_CAM_HPP
#define GLV_CAM_HPP

#include <string>

#include <fx/gltf.h>

class GLVCamera
{
private:
    enum class Type{
        None,
        Orthographic,
        Perspective
    };

    struct Orthographic{
        float xmag{};
        float ymag{};
        float zfar{};
        float znear{};
    };

    struct Perspective{
        float aspectRatio{};
        float yfov{};
        float zfar{};
        float znear{};
    };

    std::string name{};
    Type type{ Type::None };

    Orthographic orthographic;
    Perspective perspective;
public:
    GLVCamera(fx::gltf::Document const & doc, std::size_t camIndex);
    ~GLVCamera();
};

#endif
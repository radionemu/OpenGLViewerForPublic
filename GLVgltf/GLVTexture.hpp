#pragma once
#ifndef GLV_TEX_HPP
#define GLV_TEX_HPP

#include <GL/glew.h>
#include <fx/gltf.h>
#include "general/GLVFileLoad/absFile.hpp"

namespace GLVGLTF{

    class GLVTexture
    {
    private:
        fx::gltf::Document * DH;

        std::int64_t texindex{-1};
        std::int64_t srcIndex{-1};
        std::int64_t smpIndex{-1};

        GLuint GLVTextureId;
        GLuint GLVPrevUVID;

        std::filesystem::path modelPath;
        std::filesystem::path fullPath;

        int mimeType(std::string str);

        void createbyMIME(fx::gltf::Document *doc);
        void createbyMIME(std::string path);

    public:
        GLVTexture(absFile * absfile, std::int64_t texindex);
        GLVTexture(std::string imgPath);
        ~GLVTexture();

        void LoadTexture();
        void LoadTexture(std::string userPath);

        GLuint GetGLVtextureID(){return GLVTextureId;};
        GLuint GetGLVPrevUVID(){return GLVPrevUVID;}
        bool isExtern(){return (srcIndex == -1);}
    };

}


#endif
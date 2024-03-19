#pragma once
#include "source/incls.h"
#include "general/loadshader.hpp"
#include "general/GLVBuffer/FrameBuffer.hpp"
#include "general/GLVFileLoad/FileManager.hpp"
#include "general/GLVBuffer/VertexBuffer.hpp"
#include "general/GLVVertexArray/BufferLayout.hpp"
#include "general/GLVVertexArray/VertexArray.hpp"
#include "general/GLVBuffer/UniformBuffer.hpp"

namespace UV
{
    class TexturePresampler
    {
    private:
        //quad
        float quad[24] = {  
            -1.0f,  1.0f,
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f,  1.0f,
            1.0f, -1.0f,
            1.0f,  1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };	
        //Sample Resolution
        size_t SampleResolution = 1024;
        //shader
        std::unique_ptr<shaders::Shader> mShader;
        //Framebuffer
        std::unique_ptr<FrameBuffer> mFrameBuffer;
        //Vertexbuffer
        std::unique_ptr<VertexArray> mVAO;
        std::unique_ptr<BufferLayout> mPOSLayout;
        std::unique_ptr<BufferLayout> mUVLayout;
        std::unique_ptr<VertexBuffer> mVBO;
        std::unique_ptr<UniformBuffer> mUBO;
    public:
        TexturePresampler(/* args */);
        ~TexturePresampler();
        void init();
        unsigned int createTexturePreview(unsigned int texid);
    };

        
} // namespace UV

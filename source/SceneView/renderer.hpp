#pragma once
#include "source/incls.h"
#include "general/loadshader.hpp"
#include "source/SceneView/camera.hpp"
#include "general/GLVFileLoad/FileManager.hpp"
#include "general/GLVBuffer/FrameBuffer.hpp"

namespace SceneView{
    struct keyInput{
        bool IsWindowHovered;
        bool IsWindowFocused;

        bool isLeft = false;
        bool isMiddle = false;
        bool isRight = false;

        int mX =0;
        int mY =0;
    };

    class renderer
    {
    protected:
        //FileManager
            std::shared_ptr<GLVGLTF::FileManager> mFileManager;
        //Camera
            std::unique_ptr<SceneView::camera> mCamera;
        //Light?

        //Shader
            std::unique_ptr<shaders::Shader> mShader;

        //FrameBuffer
            std::unique_ptr<FrameBuffer> mFramebuffer;

        //KeyInput
            keyInput mKeyInput;
    public:
        renderer(/* args */);
        ~renderer();
        void init(std::shared_ptr<GLVGLTF::FileManager> fManager, const char* vertPath, const char* fragPath);
        // virtual void render()=0; //check later
        void predraw(unsigned int width, unsigned int height); //init the shader, framebuffer and camera setting
        void draw(); //call drawarray. There can be additional drawing method in extended classes if you want
        void postdraw(); //swap the buffer and commit to IMGUI
        void setColor(glm::vec4 color);
        void enable(size_t flags);
        void disable(size_t flags);

        virtual void onMouseDown(unsigned int MSG, int X, int Y) =0;
        virtual void onMouseClick() =0;
        virtual void onMouseUp(unsigned int MSG, int X, int Y) =0;
        virtual void onMouseMove(int X, int Y) =0;
        virtual void onMouseWheel(int Z) =0;
    };    
}
enum EnableFlags{
    EnableFlags_ = (0x00),
    EnableFlags_DEPTHTEST = (0x01<<0),
    EnableFlags_BLEND_SRC_ALPHA = (0x01<<1),
    EnableFlags_BLEND_DST_ALPHA = (0x01<<2)
};
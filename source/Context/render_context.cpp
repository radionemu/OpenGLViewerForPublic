#include "render_context.hpp"
#include "source/incls.h"
#include "abstract_context.hpp"

namespace GLVrender{
    RenderContext::~RenderContext()
    {
        //pack it up to CleanUpDeviceWGL()
        wglMakeCurrent(NULL, NULL);
        ::ReleaseDC((HWND)(mWindow->getWndHandler()),(HDC)(mWindow->getHDC()));
    }

    bool RenderContext::init(WindowManager::window *wind)
    {
        mWindow = wind;
        GLenum res = glewInit();
        if(res != GLEW_OK){
            fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
            return false;
        }

        //select the background color
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glEnable(GL_POINT_SPRITE);

        //enable Depth Test
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        //enable Culling Face

        //face culling is currently disabled for texture preview

        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);

        return true;
    }

    void RenderContext::ClearFrame()
    {
        glViewport(0,0,mWindow->Width,mWindow->Height);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void RenderContext::SwapBuffers()
    {
        ::SwapBuffers((HDC)mWindow->getHDC());
    }
}

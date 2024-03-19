#pragma once
#include "source/WindowManager/win.hpp"
#include "abstract_context.hpp"

namespace GLVrender{
    class RenderContext
    {
    private:
        WindowManager::window* mWindow;
    public:
        ~RenderContext();
        bool init(WindowManager::window* wind);
        void ClearFrame();
        void SwapBuffers();
    };
}

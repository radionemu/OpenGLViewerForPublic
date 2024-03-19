#pragma once
#include "source/WindowManager/win.hpp"

namespace GLVrender{
    class absRenderCTX{
        public:
            bool init(WindowManager::window* mwin){
                mWindow = mwin;
                return true;
            }
        protected:
            WindowManager::window* mWindow;
    };
}

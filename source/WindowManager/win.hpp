#pragma once
#include <source/incls.h>

namespace WindowManager
{
    class window{
        public :
        virtual void* getWndHandler() = 0;

        virtual void setWndHandler(void * wnd) =0;

        virtual void setHDC(void* hdc)=0;

        virtual void* getHDC() = 0;

        virtual unsigned int getIcoID(unsigned int enumCode)=0;

        int Width;
        int Height;
        std::string Path;
    };
} // namespace GLVWIN

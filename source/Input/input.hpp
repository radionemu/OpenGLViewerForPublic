#pragma once

#include <source/incls.h>

namespace Input
{
    enum class IMouseButton{
        IMouse_Left = 0,
        IMouse_Right = 1,
        IMouse_Mid = 2,
        IMouse_None = 9
    };

    class input
    {
    public:
        static IMouseButton GetPressedButtonDown(){
            return IMouseButton::IMouse_None;
        }
    };
} // namespace name

#pragma once
#include <windows.h>
#include "source/WindowManager/win.hpp"

class GLVWindow{
    private:
    public:
        void* m_handle = nullptr;
        HDC hdc;
        HGLRC con;
        WNDCLASSW wc ={};

        //temp
        float mx = 0.0f;
        float my = 0.0f;
        float mz = 0.0f;

        GLVWindow();
        ~GLVWindow();

        bool init(WindowManager::window* absWind);
        static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
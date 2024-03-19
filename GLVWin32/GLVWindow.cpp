#include "GLVWindow.hpp"
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <GL/glew.h>
#include <backends/imgui_impl_win32.h>

#include "source/WindowManager/win.hpp"

#include <iostream>
#include "GL/wgl.h"
#include "GL/wglew.h"
#include "GL/wglext.h"

#define WS_OVERLAPPEDWINDOW ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX)

GLVWindow::GLVWindow()
{
}

GLVWindow::~GLVWindow()
{
    wglMakeCurrent(NULL, NULL);
    ::ReleaseDC((HWND)m_handle, hdc);
    wglDeleteContext(con);
    ::DestroyWindow((HWND)m_handle);
    ::UnregisterClassW((LPCWSTR)wc.lpszClassName, wc.hInstance);
    DestroyWindow((HWND)m_handle);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool GLVWindow::init(WindowManager::window* absWind)
{
    //Create Application Window
    // wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = L"OpenGLViewer";
    wc.lpfnWndProc = (WNDPROC)WndProc;

    assert(RegisterClassW(&wc));

    RECT rc = {0,0,absWind->Width,absWind->Height};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
    LPCWSTR lpszClass = L"OpenGLViewer";
    m_handle = CreateWindowW(lpszClass, L"OpenGLViewer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, wc.hInstance, NULL);

    assert(m_handle);

    //Create Opengl Context
    hdc = ::GetDC((HWND)m_handle);
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);
    ::ReleaseDC((HWND)m_handle, hdc);

    hdc = ::GetDC((HWND)m_handle);
    con = wglCreateContext(hdc);

    if(!wglMakeCurrent(hdc, con))
        return 0;
    wglMakeCurrent(hdc, con);

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    // attribs 구조체는 반드시 0으로 끝나야 한다
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor,
        WGL_CONTEXT_FLAGS_ARB,
        WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB |
        WGL_CONTEXT_DEBUG_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    PFNWGLCREATECONTEXTATTRIBSARBPROC f = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
    HGLRC tmp_hRC = f(hdc, 0, attribs);
    wglMakeCurrent(0, 0);
    wglDeleteContext(con); // tmp_hRC가 제대로 얻어졌다면, 레거시 RC를 삭제한다
    con = tmp_hRC;
    wglMakeCurrent(hdc, con); // 생성된 RC를 선택한다


    ShowWindow((HWND)m_handle, SW_SHOW);
    UpdateWindow((HWND)m_handle);

    absWind->setWndHandler(m_handle);
    absWind->setHDC(hdc);

    printf("OPENGL VERSION :::: %s\n", (char *)glGetString(GL_VERSION));

    return true;
}

LRESULT WINAPI GLVWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool isRight = false;
    static int mX;
    static int mY;
    //IMGUI event
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            // g_Width = LOWORD(lParam);
            // g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

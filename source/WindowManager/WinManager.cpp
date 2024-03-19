#include "WinManager.hpp"
#include "GLVWin32/GLVWindow.hpp"
#include "general/GLVFileLoad/FileManager.hpp"

namespace WindowManager{
    WinManager::WinManager()
    {
        this->mWINctx = std::make_unique<GLVWindow>();
        this->mRenderctx = std::make_unique<GLVrender::RenderContext>();
        this->mUIctx = std::make_unique<GLVrender::UIContext>();
        this->mFileManager = std::make_shared<GLVGLTF::FileManager>();
        this->mInspector = std::make_unique<UIComponent::Inspector>();
        this->mHierarhcy = std::make_unique<UIComponent::Hierarchy>();
        this->mScene = std::make_unique<UIComponent::Scene>();
        this->mSysRsrcManager = std::make_unique<Resources::sysRsrcManager>();
    }

    void *WinManager::getWndHandler()
    {
        return this->m_handle;
    }

    void WinManager::setWndHandler(void *handler)
    {
        this->m_handle = handler;
    }

    void *WinManager::getHDC()
    {
        return this->m_HDC;
    }

    void WinManager::setHDC(void *hdc)
    {
        this->m_HDC = hdc;
    }

    unsigned int WinManager::getIcoID(unsigned int enumCode)
    {
        return this->mSysRsrcManager->getIconID(enumCode);
    }

    bool WinManager::init(int width, int height, const std::string path)
    {
        //init Class
        this->Width = width;
        this->Height = height;
        this->Path = path;

        //init WINAPI
        this->mWINctx->init(this);
        //init GLEW
        this->mRenderctx->init(this);
        //init IMGUI
        this->mUIctx->init(this);

        //init FileManager
        this->mFileManager->init(path);
        //init system resource manager
        this->mSysRsrcManager->init();

        //init SceneView
        this->mScene->init(this->mFileManager);
        //init Inspector
        this->mInspector->init(this->mFileManager, this);
        //init Hierarchy
        this->mHierarhcy->init(this);

        return false;
    }

    void WinManager::render()
    {
        input();

        //Clear Frame
        mRenderctx->ClearFrame();

        //Create base Docker Frame
        mUIctx->CreateDocker();

        //Create Hierarchy Frame
        mHierarhcy->render(mFileManager.get());

        //Create Inspector Frame
        mInspector->render(mHierarhcy->getESelected());

        //Create Scene Frame
        mScene->render();

        //Render UI
        mUIctx->RenderFrame();

        //SwapBuffer
        mRenderctx->SwapBuffers();
    }
    
    void WinManager::input()
    {
        MSG msg;
        if(::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if(msg.message == WM_RBUTTONDOWN || msg.message == WM_LBUTTONDOWN || msg.message == WM_MBUTTONDOWN){
                int X = LOWORD(msg.lParam);
                int Y = HIWORD(msg.lParam);
                mScene->onMouseDown(msg.message, X, Y);
                mInspector->onMouseDown(msg.message, X, Y);
            }
            else if(msg.message == WM_RBUTTONUP || msg.message == WM_LBUTTONUP || msg.message == WM_MBUTTONUP){
                int X = LOWORD(msg.lParam);
                int Y = HIWORD(msg.lParam);
                mScene->onMouseUp(msg.message, X, Y);
                mInspector->onMouseUp(msg.message, X, Y);
            }
            else if(msg.message == WM_MOUSEMOVE){
                int X = LOWORD(msg.lParam);
                int Y = HIWORD(msg.lParam);
                mScene->onMouseMove(X,Y);
                mInspector->onMouseMove(X,Y);
            }
            else if(msg.message == WM_MOUSEWHEEL){
                int Z = glm::sign((int)((SHORT)HIWORD(msg.wParam)));
                mScene->onMouseWheel(Z);
                mInspector->onMouseWheel(Z);
            }
        }
    }
}

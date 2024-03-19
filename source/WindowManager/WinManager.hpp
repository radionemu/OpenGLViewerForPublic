#pragma once
#include "win.hpp"
#include "GLVWin32/GLVWindow.hpp"
#include "source/Context/render_context.hpp"
#include "source/Context/imgui_context.hpp"
#include "general/GLVFileLoad/FileManager.hpp"
#include "source/UIComponent/Hierarchy.hpp"
#include "source/UIComponent/Scene.hpp"
#include "source/UIComponent/Inspector.hpp"
#include "source/Manager/sysRsrcManager.hpp"
#include <source/incls.h>


namespace WindowManager{
    class WinManager : public window
    {
    private:
        bool glv_isrunning;

        // Contexts
            void* m_handle;
            void* m_HDC;
            //Window Context(WINAPI)
            std::unique_ptr<GLVWindow> mWINctx;

            //UI Context(IMGUI)
            std::unique_ptr<GLVrender::UIContext> mUIctx;

            //Render Context(GLEW)
            std::unique_ptr<GLVrender::RenderContext> mRenderctx;

        // UI Components
            //Scene
            std::unique_ptr<UIComponent::Scene> mScene;
            //Hierarchy
            std::unique_ptr<UIComponent::Hierarchy> mHierarhcy;
            //Inspector
            std::unique_ptr<UIComponent::Inspector> mInspector;

        // Data Components
            //file manager
            std::shared_ptr<GLVGLTF::FileManager> mFileManager;
            //Data Resource Manager
            std::shared_ptr<Resources::sysRsrcManager> mSysRsrcManager;

    public:
        WinManager();

        //override
        void* getWndHandler() override;

        void setWndHandler(void * handler) override;

        void* getHDC() override;

        void setHDC(void * hdc) override;

        unsigned int getIcoID(unsigned int enumCode) override;
        //override

        bool init(int width, int height, const std::string Title);

        void render();

        void input();

        void handle_mscroll(double mDelta);

        void handle_keyboard(int key);

        void handle_resize(int width, int height);

        void close();

        bool is_running(){return glv_isrunning;};
    };
}
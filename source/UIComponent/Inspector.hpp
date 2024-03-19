#pragma once
#include "source/incls.h"
#include "GLVgltf/GLVElement.hpp"
#include "general/GLVFileLoad/FileManager.hpp"
#include "source/UV/TexturePresampler.hpp"
#include "source/UV/MatPrevRender.hpp"
#include "absInspec.hpp"
#include "source/WindowManager/win.hpp"

namespace UIComponent{
    class Inspector : public absInspector
    {
        enum TChanger{
            TChanger_BaseColor = 0x01 << (size_t)0,
            TChanger_MetalRough = 0x01 << (size_t)1,
            TChanger_Normal = 0x01 << (size_t)2,
            TChanger_Occlusion = 0x01 << (size_t)3,
            TChanger_Emission = 0x01 << (size_t)4
        };
    private:
        //FileManager instance
        std::shared_ptr<GLVGLTF::FileManager> mFManager;

        //window instance
        WindowManager::window* mWindow;        

        //Matpreview
        std::unique_ptr<UV::MaterialPreviewRenderer> mMatPrevRenderer;

        //ImGUI properties
        float labAlign = 150.0f;
        float drag_zero = 0.0f;
        float drag_one = 1.0f;
        unsigned int pushid = 0;
        unsigned int flags = 0;

        //Inspector Pointer
        GLVGLTF::GLVElement* iPointer;

        //TexturePresamepler
        std::unique_ptr<UV::TexturePresampler> UVpreSampler;
        //TexturePreviews
        std::vector<std::pair<unsigned int, unsigned int>> UVpreview; //should change to heap
        GLVGLTF::GLVTexture* tPointer = nullptr;
        GLVGLTF::GLVTexture* tPos = nullptr;//this is used at modal
    public:
        Inspector(/* args */);
        ~Inspector();

        void init(std::shared_ptr<GLVGLTF::FileManager> fManager, WindowManager::window *wind);
        void render(void* epointer);
        void renderMatInspector();
            bool renderImgBtn(unsigned int imgID, unsigned int GUID);
            bool renderImgBtn(unsigned int imgID, size_t btnsize, unsigned int GUID);
            void renderModal(size_t flags);

        std::vector<std::pair<unsigned int, unsigned int>>* getPreviewUVs()override{return &UVpreview;}
        int binarySearch(unsigned int texID) override;
        bool comp(std::pair<unsigned int, unsigned int> a, std::pair<unsigned int, unsigned int> b);
    
        //mouseInput
        void onMouseDown(unsigned int MSG, int X, int Y);
        void onMouseMove(int X, int Y);
        void onMouseWheel(int Z);
        void onMouseUp(unsigned int MSG, int X, int Y);

        //file open Dialogue
        std::string openFile();
        std::string ToString(std::wstring value);
    };
}
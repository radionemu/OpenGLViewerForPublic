#pragma once
#include "source/incls.h"
#include "general/loadshader.hpp"
#include "general/GLVFileLoad/FileManager.hpp"
#include "general/GLVBuffer/FrameBuffer.hpp"
#include "source/SceneView/camera.hpp"
#include "source/UIComponent/absInspec.hpp"
#include "source/SceneView/renderer.hpp"

namespace UV
{
    class MaterialPreviewRenderer : public SceneView::renderer
    {
    private:
        //master class
        UIComponent::absInspector* pInspector;
        std::shared_ptr<GLVGLTF::FileManager> mFileManager;
        std::shared_ptr<GLVGLTF::GLVMaterial> mMaterial;

        //Panel New
        ImVec2 cMin;
        ImVec2 cMax;
        ImVec2 wPos;

        //Panel Legacy
        glm::vec2 mPanelSize;
        std::pair<int, int> mInspectorPixelSize;
        std::vector<std::pair<unsigned int, unsigned int>>* mPreviewTexture;
    
        //trackball
        glm::vec3 curVec = glm::vec3(0.0f);
        glm::vec3 lastVec = glm::vec3(0.0f);
        glm::vec3 axis = glm::vec3(0.0f);
        int curX;
        int curY;
        bool isTrackballMove = false;

    public:
        MaterialPreviewRenderer();
        ~MaterialPreviewRenderer();
        
        void init(UIComponent::absInspector* absInspec);
        void render(GLVGLTF::GLVMaterial *pMaterial);

        //trackball method
        void ptov(int mouseX, int mouseY, float screenW, float screenH, glm::vec3& vec);
        void localizePos(int& mouseX, int&mouseY);
        void initTrackball(int X, int Y);
        void moveTrackball(int X, int Y);
        void completeTrackball(int X, int Y);

        void onMouseDown(unsigned int MSG, int X, int Y) override;
        void onMouseMove(int X, int Y) override;
        void onMouseWheel(int Z) override;
        void onMouseUp(unsigned int MSG, int X, int Y) override;
        void onMouseClick() override;
    };
} // namespace U

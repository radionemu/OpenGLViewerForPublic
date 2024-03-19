#pragma once
#include "GLVgltf/GLVElement.hpp"
#include "source/incls.h"
#include "general/GLVFileLoad/FileManager.hpp"
#include "source/WindowManager/win.hpp"
#include <unordered_map>

namespace UIComponent
{
    struct ElementSelect{ //count start with 1
        size_t eSize = 0;
        GLVGLTF::GLVElement* eSelected = nullptr; //unselected
        GLVGLTF::GLVElement* ePointer = nullptr; //current pointer
    };

    enum Toggle{
        Toggl_Scene = 0x01 << 0,
        Toggl_NodeNormal = 0x01 << 1,
        Toggl_NodeMesh = 0x01 << 2,
        Toggl_Camera = 0x01 << 3,
        Toggl_Mesh = 0x01 << 4,
        Toggl_Primitive = 0x01 << 5,
        Toggl_Material = 0x01 << 6
    };

    class Hierarchy
    {
    private:
        ImGuiTreeNodeFlags nFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        ImGuiTreeNodeFlags lFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        std::unordered_map<std::string, size_t> nameSet;

        //currently selected element instance
        ElementSelect eSelector;

        //currently toggled filter
        size_t mFilter = 0x00;

        WindowManager::window* mWindow;
    public:
        Hierarchy(/* args */);
        ~Hierarchy();

        void init(WindowManager::window* Win);

        void render(GLVGLTF::FileManager * fManager);
        void renderToggle();
        void renderFile(GLVGLTF::GLVFile * glFile);
        void renderScene(GLVGLTF::GLVScene * glScene);
        void renderNode(GLVGLTF::GLVNode * glNode);
        void renderMesh(GLVGLTF::GLVMeshes * glMesh);
        void renderPrimitive(GLVGLTF::GLVPrimitive * glPrim);
        void renderMaterial(GLVGLTF::GLVMaterial * glMat);
        
        bool renderImgbtn(unsigned int imgID);
        void* getESelected(){return eSelector.eSelected;}
    };
}

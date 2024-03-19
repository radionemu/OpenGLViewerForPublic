#include "Hierarchy.hpp"
#include "source/incls.h"
#include "GLVgltf/GLVElement.hpp"
#include "general/GLVFileLoad/FileManager.hpp"
#include "source/Manager/sysRsrcManager.hpp"

using namespace GLVGLTF;

namespace UIComponent{
    Hierarchy::Hierarchy()
    {
    }

    Hierarchy::~Hierarchy()
    {
    }

    void Hierarchy::init(WindowManager::window *Win)
    {
        this->mWindow = Win;
        // this->mFilter = mFilter ^ (size_t)127;
        this->mFilter = mFilter ^ (size_t)64;
    }

    bool Hierarchy::renderImgbtn(unsigned int imgID)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
        ImVec2 size = ImVec2(16.0f, 16.0f);                         // size of image must fit to fontsize
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
        ImVec2 uv1 = ImVec2(1.0f, 1.0f);    // UV coordinates for (32,32) in our texture
        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);             // Black background
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
        bool retval = (ImGui::ImageButton("##1", reinterpret_cast<void *>(imgID), size, uv0, uv1, bg_col, tint_col));
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        return retval;
    }

    void Hierarchy::render(GLVGLTF::FileManager *fManager)
    {
        nameSet.clear();
        ImGui::Begin("Hierarhcy");
        //filter toggle

        renderToggle();
        //
        for(GLVFile *glvfile : *fManager->getfLists()){
            renderFile(glvfile);
        }

        ImGui::End();
    }

    void Hierarchy::renderToggle()
    {
        ImVec4 ActCol = ImGui::GetStyleColorVec4((ImGuiCol)ImGuiCol_ButtonActive);
        ImVec4 NonCol = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
        // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImVec2 size = ImVec2(32.0f, 32.0f);                         // size of image must fit to fontsize
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
        ImVec2 uv1 = ImVec2(1.0f, 1.0f);    // UV coordinates for (32,32) in our texture
        ImVec4 bg_col;             // Black background
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
            if(mFilter & Toggl_Scene) bg_col = ActCol;
            else bg_col = NonCol;
            if(ImGui::ImageButton("##1", reinterpret_cast<void *>(this->mWindow->getIcoID(icons_Scene_Color)), size, uv0, uv1, bg_col, tint_col))
                mFilter = mFilter ^ Toggl_Scene;
            ImGui::SameLine();
            if(mFilter & Toggl_NodeNormal) bg_col = ActCol;
            else bg_col = NonCol;
            if(ImGui::ImageButton("##2", reinterpret_cast<void *>(this->mWindow->getIcoID(icons_NodeNormal_Color)), size, uv0, uv1, bg_col, tint_col))
                mFilter ^= Toggl_NodeNormal;  
            ImGui::SameLine();
            if(mFilter & Toggl_NodeMesh) bg_col = ActCol;
            else bg_col = NonCol;
            if(ImGui::ImageButton("##3", reinterpret_cast<void *>(this->mWindow->getIcoID(icons_NodeMesh_Color)), size, uv0, uv1, bg_col, tint_col))
                mFilter ^= Toggl_NodeMesh;
            ImGui::SameLine();
            if(mFilter & Toggl_Camera) bg_col = ActCol;
            else bg_col = NonCol;
            if(ImGui::ImageButton("##4", reinterpret_cast<void *>(this->mWindow->getIcoID(icons_Camera_Color)), size, uv0, uv1, bg_col, tint_col))
                mFilter ^= Toggl_Camera;
            ImGui::SameLine();
            if(mFilter & Toggl_Mesh) bg_col = ActCol;
            else bg_col = NonCol;
            if(ImGui::ImageButton("##5", reinterpret_cast<void *>(this->mWindow->getIcoID(icons_Mesh_Color)), size, uv0, uv1, bg_col, tint_col))
                mFilter ^= Toggl_Mesh;
            ImGui::SameLine();
            if(mFilter & Toggl_Primitive) bg_col = ActCol;
            else bg_col = NonCol;
            if(ImGui::ImageButton("##6", reinterpret_cast<void *>(this->mWindow->getIcoID(icons_Primitive_Color)), size, uv0, uv1, bg_col, tint_col))
                mFilter ^= Toggl_Primitive;      
            ImGui::SameLine();
            if(mFilter & Toggl_Material) bg_col = ActCol;
            else bg_col = NonCol;
            if(ImGui::ImageButton("##7", reinterpret_cast<void *>(this->mWindow->getIcoID(icons_Material_Color)), size, uv0, uv1, bg_col, tint_col))
                mFilter ^= Toggl_Material;

        // ImGui::PopStyleColor();
        ImGui::PopStyleVar(1);
    }

    void Hierarchy::renderFile(GLVGLTF::GLVFile *glFile)
    {
        ImGui::SeparatorText(glFile->getFileName().c_str());
        for(GLVScene * scene : *(glFile->getScenes()))
        {
            renderScene(scene);
        }
    }

    void Hierarchy::renderScene(GLVGLTF::GLVScene * glScene)
    {
        //eSelect
        eSelector.ePointer = glScene;

        //flags
        ImGuiTreeNodeFlags flags = this->nFlags;
        if(glScene->getrtNodeNum() <= 0)flags = this->lFlags;
        if(!(mFilter & (Toggl_Material|Toggl_Primitive|Toggl_Mesh|Toggl_NodeMesh|Toggl_NodeNormal)))flags=this->lFlags;

        //nameidentifier
        std::unordered_map<std::string, size_t>::iterator pos;
        std::string scenename = glScene->getElementName();
        pos = this->nameSet.find(scenename);
        size_t n = 0;
        if(pos == nameSet.end()){
            nameSet.insert({scenename, n});
        }else{
            n = ++pos->second;
        }

        //add
        if(mFilter & Toggl_Scene){
            bool isClicked = ImGui::TreeNodeEx((glScene->getElementName()+"_"+std::to_string(n)).c_str(), flags | (eSelector.ePointer == eSelector.eSelected ? ImGuiTreeNodeFlags_Selected : 0));//doubleClicked
            if(ImGui::IsItemClicked()){//singleClicked
                eSelector.eSelected = glScene;
            }
            ImGui::SameLine();
            renderImgbtn(mWindow->getIcoID(icons_Scene_Color));
            if(isClicked && flags == nFlags){
                for(GLVNode * node : *glScene->getRTNode()){
                    renderNode(node);
                }
                ImGui::TreePop();
            }        
        }else{
            for(GLVNode * node : *glScene->getRTNode()){
                renderNode(node);
            }
        }
    }

    void Hierarchy::renderNode(GLVGLTF::GLVNode *glNode)
    {
        eSelector.ePointer = glNode;
        bool hasMsh = glNode->hasMesh();

        //flags
        ImGuiTreeNodeFlags flags = this->nFlags;
        if(!glNode->hasChild() && !glNode->hasMesh())flags = this->lFlags;
        if(!(mFilter & (Toggl_Material | Toggl_Primitive | Toggl_Mesh)))flags = this->lFlags;

        if(mFilter & Toggl_NodeNormal && !hasMsh){
            bool isClicked = ImGui::TreeNodeEx(glNode->getElementName().c_str(), flags | (eSelector.ePointer == eSelector.eSelected ? ImGuiTreeNodeFlags_Selected : 0));
            if(ImGui::IsItemClicked()){//singleClicked
                eSelector.eSelected = glNode;
            }
            ImGui::SameLine();
            renderImgbtn(mWindow->getIcoID(icons_NodeNormal_Color));
            if(isClicked && glNode->hasChild() && flags == nFlags){
                for(GLVNode * cNode : *glNode->getChildNodes()){
                    renderNode(cNode);
                }
                ImGui::TreePop();
            }
        }
        else if(mFilter & Toggl_NodeMesh && hasMsh){
            bool isClicked = ImGui::TreeNodeEx(glNode->getElementName().c_str(), flags | (eSelector.ePointer == eSelector.eSelected ? ImGuiTreeNodeFlags_Selected : 0));
            if(ImGui::IsItemClicked()){//singleClicked
                eSelector.eSelected = glNode;
            }
            ImGui::SameLine();
            renderImgbtn(mWindow->getIcoID(icons_NodeMesh_Color));
            if(isClicked && glNode->hasMesh() && flags == nFlags){
                renderMesh(glNode->getMesh());
                ImGui::TreePop();
            }
        }else{
            if(glNode->hasChild()){
                for(GLVNode * cNode : *glNode->getChildNodes()){
                    renderNode(cNode);
                }
            }else if(glNode->hasMesh()){
                renderMesh(glNode->getMesh());
            }

        }
    }
    void Hierarchy::renderMesh(GLVGLTF::GLVMeshes *glMesh)
    {
        eSelector.ePointer = glMesh;

        ImGuiTreeNodeFlags flags = this->nFlags;
        if(!glMesh->hasPrimitive())flags = this->lFlags;
        if(!(mFilter & (Toggl_Material | Toggl_Primitive)))flags = this->lFlags;

        if(mFilter & Toggl_Mesh){
            bool isClicked = ImGui::TreeNodeEx(glMesh->getElementName().c_str(), flags | (eSelector.ePointer == eSelector.eSelected ? ImGuiTreeNodeFlags_Selected : 0));
            if(ImGui::IsItemClicked()){//singleClicked
                eSelector.eSelected = glMesh;
            }
            ImGui::SameLine();
            renderImgbtn(mWindow->getIcoID(icons_Mesh_Color));
            //renderChilde
            if(isClicked && flags == nFlags){
                for(GLVPrimitive * prim : *glMesh->getPrimitives()){
                    renderPrimitive(prim);
                }
                ImGui::TreePop();
            }
        }
        else{
            for(GLVPrimitive * prim : *glMesh->getPrimitives()){
                renderPrimitive(prim);
            }
        }

    }
    void Hierarchy::renderPrimitive(GLVGLTF::GLVPrimitive *glPrim)
    {
        eSelector.ePointer = glPrim;

        ImGuiTreeNodeFlags flags = this->nFlags;
        if(!glPrim->hasMaterial())flags = this->lFlags;
        if(!(mFilter & Toggl_Material))flags = this->lFlags;

        if(mFilter & Toggl_Primitive){
            bool isClicked = ImGui::TreeNodeEx(glPrim->getElementName().c_str(), flags | (eSelector.ePointer == eSelector.eSelected ? ImGuiTreeNodeFlags_Selected : 0));
            if(ImGui::IsItemClicked()){//singleClicked
                eSelector.eSelected = glPrim;
            }
            ImGui::SameLine();
            renderImgbtn(mWindow->getIcoID(icons_Primitive_Color));

            //render Child
            if(isClicked && flags == nFlags){
                renderMaterial(glPrim->getMaterial());
                ImGui::TreePop();
            }
        }else{
            if(glPrim->hasMaterial()){
                renderMaterial(glPrim->getMaterial());
            }
        }

    }
    void Hierarchy::renderMaterial(GLVGLTF::GLVMaterial *glMat)
    {
        eSelector.ePointer = glMat;

        ImGuiTreeNodeFlags flags = this->lFlags;

        if(mFilter & Toggl_Material){
            bool isClicked = ImGui::TreeNodeEx(glMat->getElementName().c_str(), flags | (eSelector.ePointer == eSelector.eSelected ? ImGuiTreeNodeFlags_Selected : 0));
            if(ImGui::IsItemClicked()){//singleClicked
                eSelector.eSelected = glMat;
            }
            ImGui::SameLine();
            renderImgbtn(mWindow->getIcoID(icons_Material_Color));
        }
    }
}

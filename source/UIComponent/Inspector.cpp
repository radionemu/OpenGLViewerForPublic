#include "Inspector.hpp"
#include "source/incls.h"
#include "GLVgltf/GLVMaterial.hpp"
#include <iostream>

//for openFiledialog
#include <commdlg.h>


namespace UIComponent{
    Inspector::Inspector()
    {
        this->mMatPrevRenderer = std::make_unique<UV::MaterialPreviewRenderer>();
    }

    Inspector::~Inspector()
    {
    }

    void Inspector::init(std::shared_ptr<GLVGLTF::FileManager> fManager, WindowManager::window *wind){
        mWindow = wind;
        mFManager = fManager;
        this->UVpreSampler = std::make_unique<UV::TexturePresampler>();
        this->UVpreSampler->init();
        for(GLVGLTF::GLVFile* iFile : fManager->fLists){
            for(GLVGLTF::GLVMaterial* iMat : *reinterpret_cast<std::vector<GLVGLTF::GLVMaterial *>*>(iFile->getMaterialList())){
                if(!iMat->isBaseClrUVEmpty()){
                    unsigned int texid = iMat->getBaseClrUV();
                    this->UVpreview.emplace_back(std::pair<unsigned int, unsigned int>({texid, UVpreSampler->createTexturePreview(texid)}));
                    //add this to Material instance?
                }
                if(!iMat->isMetalRoughUVEmpty()){
                    unsigned int MRid = iMat->getMetRoughUV();
                    this->UVpreview.emplace_back(std::pair<unsigned int, unsigned int>({MRid, UVpreSampler->createTexturePreview(MRid)}));
                }
                if(!iMat->isNormalUVEmpty()){
                    unsigned int normalID = iMat->getNormalUV();
                    this->UVpreview.emplace_back(std::pair<unsigned int, unsigned int>({normalID, UVpreSampler->createTexturePreview(normalID)}));
                }
                if(!iMat->isOcclusionUVEmpty()){
                    unsigned int occID = iMat->getOcclusionUV();
                    this->UVpreview.emplace_back(std::pair<unsigned int, unsigned int>({occID, UVpreSampler->createTexturePreview(occID)}));
                }
                if(!iMat->isEmissiveUVEmpty()){
                    unsigned int EmID = iMat->getEmissiveUV();
                    this->UVpreview.emplace_back(std::pair<unsigned int, unsigned int>({EmID, UVpreSampler->createTexturePreview(EmID)}));
                }
            }
        }
        this->UVpreview.emplace_back(std::pair<unsigned int, unsigned int>({-1, UVpreSampler->createTexturePreview(-1)}));
        
        //sort
        std::sort(UVpreview.begin(), UVpreview.end());//check if sort is not completed
        for(int i=0; i<UVpreview.size(); i++){
            printf("UVPREV [[%d||%d]]\t",UVpreview[i].first,UVpreview[i].second);
            printf("BS [[%d||%d]]\n", UVpreview[i].first, binarySearch(UVpreview[i].first));
        }
    
        //init material preview frame buffer
        this->mMatPrevRenderer->init(this);
        
    }

    void Inspector::render(void* epointer)
    {
        this->iPointer = (GLVGLTF::GLVElement*)epointer;

        ImGui::Begin("Inspector");
        if(iPointer!=nullptr){
            ImGui::SeparatorText(iPointer->getElementName().c_str());
            if(typeid(*iPointer) == typeid(GLVGLTF::GLVMaterial)){//typeid?
                renderMatInspector();
            }
        }
        ImGui::End();
    }

    void Inspector::renderMatInspector()
    {
        bool isOpen = false;
        GLVGLTF::GLVMaterial *eMatPointer = static_cast<GLVGLTF::GLVMaterial *>(iPointer);
        //render preview
        mMatPrevRenderer->render(eMatPointer);
        //Material
                    //begin table
            if(ImGui::BeginTable("Main Maps", 3)){
            unsigned int TEXID;
                //baseColorTexture
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Base color");
                ImGui::TableNextColumn();
                    TEXID = -1;
                    if(!eMatPointer->isBaseClrUVEmpty()){
                        TEXID = eMatPointer->getBaseClrUV();
                    }if(!eMatPointer->isBaseClrTmpEmpty()){
                        TEXID = eMatPointer->getBaseClrUVTmp();
                    }   
                bool isBSCclicked = renderImgBtn(TEXID, TChanger_BaseColor);
                if(isBSCclicked) {
                    flags = TChanger_BaseColor;
                    isOpen = true;   
                }

                ImGui::TableNextColumn();
                ImGui::ColorEdit4("##1", (float *)(&(eMatPointer->getBaseClrFactor())[0]), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                //Metallic
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Metallic");  
                ImGui::TableNextColumn();      
                    TEXID = -1;    
                    if(!eMatPointer->isMetalRoughUVEmpty()){
                        TEXID = eMatPointer->getMetRoughUV();
                    }if(!eMatPointer->isMetalRoughTmpEmpty()){
                        TEXID = eMatPointer->getMetalRoughUVTmp();
                    }
                    bool isMRclicked = renderImgBtn(TEXID, TChanger_MetalRough);
                    if(isMRclicked) {
                        flags = TChanger_MetalRough;
                        isOpen = true;
                    }
                ImGui::TableNextColumn();
                ImGui::DragScalar("##2",ImGuiDataType_Float,(eMatPointer->getMetallic()), 0.005f,  &(drag_zero), &(drag_one), "%f");
            
                //Roughness
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Roughness");
                ImGui::TableNextColumn();
                ImGui::TableNextColumn();
                ImGui::DragScalar("##3",ImGuiDataType_Float,eMatPointer->getRoughness(), 0.005f,  &(drag_zero), &(drag_one), "%f");
            
                //Normal
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Normal");
                ImGui::TableNextColumn();
                TEXID = -1;
                if(!eMatPointer->isNormalUVEmpty())
                    TEXID = eMatPointer->getNormalUV();
                if(!eMatPointer->isNormalTmpEmpty())
                    TEXID = eMatPointer->getNormalUVTmp();
                bool isNMclicked = renderImgBtn(TEXID, TChanger_Normal);
                if(isNMclicked) {
                    flags = TChanger_Normal;
                    isOpen = true;
                }
                ImGui::TableNextColumn();
                ImGui::DragScalar("Scale",ImGuiDataType_Float,eMatPointer->getNormalScale(), 0.005f,  &(drag_zero), &(drag_one), "%f");
            
                //Occlusion
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Occlusion");
                ImGui::TableNextColumn();
                TEXID = -1;
                if(!eMatPointer->isOcclusionUVEmpty())
                    TEXID = eMatPointer->getOcclusionUV();
                if(!eMatPointer->isOcclusionTmpEmpty())
                    TEXID = eMatPointer->getOcclusionUVTmp();
                bool isOCclicked = renderImgBtn(TEXID,TChanger_Occlusion);
                if(isOCclicked) {
                    flags = TChanger_Occlusion;
                    isOpen = true;
                }
                ImGui::TableNextColumn();
                ImGui::DragScalar("Strength",ImGuiDataType_Float,eMatPointer->getOcclusionStren(), 0.005f,  &(drag_zero), &(drag_one), "%f");

                //Emissive
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Emissive");
                ImGui::TableNextColumn();
                TEXID = -1;
                if(!eMatPointer->isEmissiveUVEmpty()){
                    TEXID = eMatPointer->getEmissiveUV();
                }
                if(!eMatPointer->isEmissiveTmpEmpty()){
                        TEXID = eMatPointer->getEmissiveUVTmp();
                }
                bool isEMclicked = renderImgBtn(TEXID,TChanger_Emission);
                if(isEMclicked) {
                    flags = TChanger_Emission;
                    isOpen = true;
                }
                ImGui::TableNextColumn();
                ImGui::ColorEdit4("Emissive Factor", (float *)(&(eMatPointer->getEmissiveFac())[0]), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::EndTable();
            }
            if(ImGui::Button("Revert"))eMatPointer->RevertTmp();
            ImGui::SameLine();
            if(ImGui::Button("Apply"))eMatPointer->ApplyTmp();
            
            //modal render
            if(isOpen){
                ImGui::OpenPopup("Select Texture");
            }
            renderModal(flags);
    }

    bool Inspector::renderImgBtn(unsigned int imgID, unsigned int GUID)
    {
        return renderImgBtn(imgID, 32, GUID);
    }

    bool Inspector::renderImgBtn(unsigned int imgID, size_t btnsize, unsigned int GUID)
    {
        ImGui::PushID(GUID);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
        ImVec2 size = ImVec2(btnsize, btnsize);                         // Size of the image we want to make visible
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
        ImVec2 uv1 = ImVec2(1.0f, 1.0f);    // UV coordinates for (32,32) in our texture
        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);             // Black background
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
        bool retval = (ImGui::ImageButton("", reinterpret_cast<void *>(imgID), size, uv0, uv1, bg_col, tint_col));
        ImGui::PopStyleVar();
        ImGui::PopID();
        return retval;
    }

    void Inspector::renderModal(size_t flags)
    {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if(ImGui::BeginPopupModal("Select Texture")){
            //from Texture Library
            for(GLVGLTF::GLVFile* glFile : *mFManager->getfLists()){
                ImGui::SeparatorText(glFile->getFileName().c_str());
                ImGuiStyle& style = ImGui::GetStyle();
                int buttons_count = 20;
                float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
                int n = 0;
                for(GLVGLTF::GLVTexture * Tex : *glFile->getTextures()){
                    ImVec2 button_sz = ImVec2(64,64);
                    if(renderImgBtn(Tex->GetGLVPrevUVID(), button_sz.x, Tex->GetGLVtextureID())){
                        tPos = Tex;
                    }
                    float last_button_x2 = ImGui::GetItemRectMax().x;
                    float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
                    if (n + 1 < glFile->getTextureNum() && next_button_x2 < window_visible_x2)
                        ImGui::SameLine();
                    n++;
                }
            }
            //from File
            ImGui::SeparatorText("External Texture");
            if(tPointer != nullptr){
                ImGui::PushID(tPointer->GetGLVtextureID());
                if(renderImgBtn(tPointer->GetGLVPrevUVID(), 64, tPointer->GetGLVPrevUVID())){
                    tPos = tPointer;
                }
                ImGui::PopID();
            }
            if(ImGui::Button("Import..")){
                std::string str = openFile();
                if(str != ""){
                    if(tPointer == nullptr){
                        tPointer = new GLVGLTF::GLVTexture(str);
                        tPos = tPointer;
                    }else{
                        tPointer->LoadTexture(str);
                        tPos = tPointer; // set Default
                    }
                }
            }


            //save or cancel
            ImGui::BeginTable("##save", 2);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
                if(ImGui::Button("cancel")){
                    ImGui::CloseCurrentPopup();
                    if(tPointer != nullptr){
                        delete tPointer;
                        tPointer = nullptr;
                        tPos = nullptr;
                    }
                    flags = 0;
                }
            ImGui::TableNextColumn();
                if(ImGui::Button("apply")){
                    ImGui::CloseCurrentPopup();
                    GLVGLTF::GLVMaterial *eMatPointer = static_cast<GLVGLTF::GLVMaterial *>(iPointer);
                    if(flags == TChanger_BaseColor)
                        eMatPointer->setBaseClrTmp(tPos);
                    else if(flags == TChanger_MetalRough)
                        eMatPointer->setMetalRoughTmp(tPos);
                    else if(flags == TChanger_Normal)
                        eMatPointer->setNormalTmp(tPos);
                    else if(flags == TChanger_Occlusion)
                        eMatPointer->setOcclusionTmp(tPos);
                    else if(flags == TChanger_Emission)
                        eMatPointer->setEmissiveTmp(tPos);
                    if(tPos != tPointer)
                        delete tPointer;
                    tPointer = nullptr;
                    tPos = nullptr;
                    flags = 0;
                }
            ImGui::EndTable();

            ImGui::EndPopup();
        }
    }

    int Inspector::binarySearch(unsigned int texID)
    {
        int low = 0;
        int hi = UVpreview.size()-1;
        while(low<=hi){
            int mid = (low+hi)/2;
            if(texID == UVpreview[mid].first)
                return UVpreview[mid].second;
            else if (texID < UVpreview[mid].first)
                hi = mid-1;
            else   
                low = mid+1;
        }
        return -1;
    }

    bool Inspector::comp(std::pair<unsigned int, unsigned int> a, std::pair<unsigned int, unsigned int> b)
    {
        return a.second > b.second;
    }
    
    void Inspector::onMouseDown(unsigned int MSG, int X, int Y)
    {
        //Material Preview
        mMatPrevRenderer->onMouseDown(MSG, X, Y);
    }
    
    void Inspector::onMouseMove(int X, int Y)
    {
        mMatPrevRenderer->onMouseMove(X,Y);
    }
    
    void Inspector::onMouseWheel(int Z)
    {
        mMatPrevRenderer->onMouseWheel(Z);
    }
    
    void Inspector::onMouseUp(unsigned int MSG, int X, int Y)
    {
        mMatPrevRenderer->onMouseUp(MSG, X, Y);
    }
    
    std::string Inspector::openFile()
    {
        OPENFILENAME OFN;
        TCHAR filePathName[100] = "";
        CHAR lpstrFile[260] = "";
        static TCHAR filter[] = "PNG format (.png)\0*.png\0JPG format (.jpg)\0*.jpg\0DDS format (.dds)\0*.dds\0any format (.*)\0*.*";
    
        memset(&OFN, 0, sizeof(OPENFILENAME));
        OFN.lStructSize = sizeof(OPENFILENAME);
        OFN.hwndOwner = (HWND)mWindow->getWndHandler();
        OFN.lpstrFilter = filter;
        OFN.lpstrFile = lpstrFile;
        OFN.nMaxFile = sizeof(lpstrFile);
        OFN.lpstrInitialDir = ".";
        std::string dir = "";

        if (GetOpenFileName(&OFN) != 0) {
            // wsprintf(filePathName, "%s Open File", OFN.lpstrFile);
            // MessageBox((HWND)mWindow->getWndHandler(), filePathName, "Select", MB_OK);
    
            dir = std::string(OFN.lpstrFile);
            //open texture

            int a = 0;
        }else{
            wsprintf(filePathName, "Open Failed");
            MessageBox((HWND)mWindow->getWndHandler(), filePathName, "Select", MB_OK);
        }
        return dir;
    }
    std::string Inspector::ToString(std::wstring value)
    {
        std::string temp;
        temp.assign(value.begin(), value.end());
        return temp;
    }
}

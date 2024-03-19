#include "MatPrevRender.hpp"
#include "source/incls.h"

namespace UV
{
    MaterialPreviewRenderer::MaterialPreviewRenderer()
    {
        this->mFileManager = std::make_shared<GLVGLTF::FileManager>();
    }

    MaterialPreviewRenderer::~MaterialPreviewRenderer()
    {
    }

    void MaterialPreviewRenderer::init(UIComponent::absInspector* absInspec)
    {
        this->pInspector = absInspec;
        this->mPreviewTexture = absInspec->getPreviewUVs();
        this->mFileManager->init("./Models/Sphere/sphere.gltf");
    
        SceneView::renderer::init(mFileManager,"./shaders/SphereMapping/Vertex.vert","./shaders/SphereMapping/Fragment.frag" );
        this->mCamera->initFront(mFileManager->getmMax(), mFileManager->getmMin());
    }

    void MaterialPreviewRenderer::render(GLVGLTF::GLVMaterial *pMaterial)
    {
        //set material
        for(GLVGLTF::GLVFile* nFile: this->mFileManager->fLists){
            for(GLVGLTF::GLVPrimitive* nPrim: *nFile->getPrimitives()){
                nPrim->setMaterial(pMaterial);
            }
        }
    
        ImGui::BeginChild("Preview", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x), ImGuiChildFlags_None, ImGuiWindowFlags_None);
        cMin = ImGui::GetWindowContentRegionMin();
        cMax = ImGui::GetWindowContentRegionMax();
        wPos = ImGui::GetWindowPos();
        // printf("vMin %f %f // vMax %f %f // wPos %f %f\n", cMin.x, cMin.y, cMax.x, cMax.y, wPos.x, wPos.y);

        ImVec2 PanelSize = ImGui::GetContentRegionAvail();
        this->mPanelSize = {PanelSize.x, PanelSize.y};
        this->mInspectorPixelSize = {static_cast<int>(PanelSize.x),static_cast<int>(PanelSize.y)};

        size_t flags = EnableFlags_DEPTHTEST | EnableFlags_BLEND_SRC_ALPHA;
        setColor(glm::vec4(0.0f));
        enable(flags);
        if(mInspectorPixelSize.first>0 && mInspectorPixelSize.second>0)//check before framebuffer calculate actual size
        {
            predraw(mInspectorPixelSize.first, mInspectorPixelSize.first);
            draw();
            postdraw();
        }    
        disable(flags);

        // //mouse input check
        // ImGuiIO& io = ImGui::GetIO();
        // onMouseDown(0);
        // if(ImGui::IsMousePosValid())
        //     onMouseMove(io.MousePos.x, io.MousePos.y);

        ImGui::EndChild();
    }
    
    void MaterialPreviewRenderer::ptov(int mouseX, int mouseY, float screenW, float screenH, glm::vec3& vec)
    {
        vec[0]=((2.0f*(float)mouseX)/(screenW))-1.0f;//normalized coord x
        vec[1]=1.0f-(((float)mouseY*2.0f)/(screenH));//normalized coord y
        float d = 1.0f-(vec[0]*vec[0]+vec[1]*vec[1]);//distance from (0,0)
        vec[2]= d>0 ? glm::sqrt(d):0.0f;//calculate z
        glm::normalize(vec);
    }

    void MaterialPreviewRenderer::localizePos(int &mouseX, int &mouseY)
    {
        //mouseX, mouseY is global WindowPosition
        mouseX = mouseX-static_cast<int>(wPos.x);
        mouseY = mouseY-static_cast<int>(wPos.y);
    }

    void MaterialPreviewRenderer::initTrackball(int X, int Y)
    {
        curX = X;
        curY = Y;
        ptov(X,Y,mPanelSize.x, mPanelSize.x, lastVec);
        
        isTrackballMove = true;
    }

    void MaterialPreviewRenderer::moveTrackball(int X, int Y)
    {
        ptov(X,Y, mPanelSize.x, mPanelSize.x, curVec);
        glm::vec3 delta = curVec - lastVec;
        // printf("curVec : %f %f %f || lastVec : %f %f %f\n", curVec.x, curVec.y, curVec.z, lastVec.x, lastVec.y, lastVec.z);
        // if(glm::length(delta)>0){
            float angle = glm::length(delta)*3.0f/(glm::half_pi<float>());
            axis = glm::cross(lastVec, curVec);
            
            lastVec = curVec;
            mCamera->rotateWorldRot(axis, angle);
        // }
    }

    void MaterialPreviewRenderer::completeTrackball(int X, int Y)
    {
        mCamera->acceptWorldRot();
    }

    void MaterialPreviewRenderer::onMouseDown(unsigned int MSG, int X, int Y)
    {
        //need to convert fullscreen mouse pos -> materialPreview mouse position
        switch (MSG)
        {
        case WM_LBUTTONDOWN:{
            this->mKeyInput.isLeft = true;
            localizePos(X,Y);
            initTrackball(X,Y);
            break;
        }
        case WM_RBUTTONDOWN:
            this->mKeyInput.isRight = true;
            break;
        default:
            break;
        }
    }



    void MaterialPreviewRenderer::onMouseMove(int X, int Y)
    {
        if(!mKeyInput.IsWindowHovered){
            // mCamera->acceptWorldRot();
            return;
        }
        if(mKeyInput.isLeft){
            localizePos(X,Y);
            moveTrackball(X,Y);
        }else{
            // mCamera->acceptWorldRot();
        }

        //update keyInput
        mKeyInput.mX = X;
        mKeyInput.mY = Y;
    }

    void MaterialPreviewRenderer::onMouseWheel(int Z)
    {
    }

    void MaterialPreviewRenderer::onMouseUp(unsigned int MSG, int X, int Y)
    {
        switch (MSG)
        {
        case WM_LBUTTONUP:{
            this->mKeyInput.isLeft = false;
            mCamera->acceptWorldRot();
            break;
        }
        case WM_RBUTTONUP:{
            this->mKeyInput.isRight = false;
            break;
        }
        default:
            break;
        }
    }
    void MaterialPreviewRenderer::onMouseClick()
    {
    }
} // namespace UV

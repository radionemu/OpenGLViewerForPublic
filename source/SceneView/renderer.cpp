#include "renderer.hpp"

SceneView::renderer::renderer()
{
    this->mShader = std::make_unique<shaders::Shader>();
    this->mCamera = std::make_unique<SceneView::camera>();        
    this->mFramebuffer = std::make_unique<FrameBuffer>();
}

SceneView::renderer::~renderer()
{
}

void SceneView::renderer::init(std::shared_ptr<GLVGLTF::FileManager> fManager, const char* vertPath, const char* fragPath)
{
        //Link Filemanager
        this->mFileManager = fManager;

        //init Shader
        this->mShader->init();
        this->mShader->LoadVF_GLSL(vertPath, fragPath);
        
        //init FrameBuffer
        this->mFramebuffer->initBuffer(1,1);

        //init Camera
        this->mCamera->init(mFileManager->getmMax(), mFileManager->getmMin());
        printf("MAX %f %f %f \t MIN %f %f %f\n", mFileManager->getmMax()[0],mFileManager->getmMax()[1],mFileManager->getmMax()[2], mFileManager->getmMin()[0],mFileManager->getmMin()[1],mFileManager->getmMin()[2]);
}

void SceneView::renderer::predraw(unsigned int width, unsigned int height)
{
    //update state
    mKeyInput.IsWindowHovered = ImGui::IsWindowHovered();
    mKeyInput.IsWindowFocused = ImGui::IsWindowFocused();

    //init buffer and bind
    this->mFramebuffer->initBuffer(width, height);
    this->mFramebuffer->Bind();
    mCamera->updateProjection(width, height);

    //camera delta update
    mCamera->updateMVP();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void SceneView::renderer::draw()
{
    //use renderer shader
    this->mShader->useProgram();
    //Primitive Queue
    for(GLVGLTF::GLVFile* nFile : this->mFileManager->fLists){//foreach file
        //load primitive
        for(GLVGLTF::GLVPrimitive *nPrim : *nFile->getPrimitives()){
            glm::vec4 pbrc = glm::vec4(0.0f);
            if(nPrim->hasMaterial()){
                GLVGLTF::GLVMaterial *nMat = nPrim->getMaterial();
                //albedo
                if(!nMat->isBaseClrUVEmpty()){
                    glActiveTexture(GL_TEXTURE0);
                    if(!nMat->isBaseClrTmpEmpty()){
                        glBindTexture(GL_TEXTURE_2D, nMat->getBaseClrUVTmp());
                    }else{
                        glBindTexture(GL_TEXTURE_2D, nMat->getBaseClrUV());
                    }
                    glUniform1i(10,0);
                }
                //metalrough
                if(!nMat->isMetalRoughUVEmpty()){
                    glActiveTexture(GL_TEXTURE1);
                    if(!nMat->isMetalRoughTmpEmpty()){
                        glBindTexture(GL_TEXTURE_2D, nMat->getMetalRoughUVTmp());
                    }else{
                        glBindTexture(GL_TEXTURE_2D, nMat->getMetRoughUV());
                    }
                    glUniform1i(11,1);
                }
                //normal
                if(!nMat->isNormalUVEmpty()){
                    glActiveTexture(GL_TEXTURE2);
                    if(!nMat->isNormalTmpEmpty()){
                        glBindTexture(GL_TEXTURE_2D, nMat->getNormalUVTmp());
                    }else{
                        glBindTexture(GL_TEXTURE_2D, nMat->getNormalUV());
                    }
                    glUniform1i(12,2);
                }
                //occlusion
                if(!nMat->isOcclusionUVEmpty()){
                    glActiveTexture(GL_TEXTURE3);
                    if(!nMat->isOcclusionTmpEmpty()){
                        glBindTexture(GL_TEXTURE_2D, nMat->getOcclusionUVTmp());
                    }else{
                        glBindTexture(GL_TEXTURE_2D, nMat->getOcclusionUV());
                    }
                    glUniform1i(13,3);
                }
                //emissive
                if(!nMat->isEmissiveUVEmpty()){
                    glActiveTexture(GL_TEXTURE4);
                    if(!nMat->isEmissiveTmpEmpty()){
                        glBindTexture(GL_TEXTURE_2D, nMat->getEmissiveUVTmp());
                    }else{
                        glBindTexture(GL_TEXTURE_2D, nMat->getEmissiveUV());
                    }
                    glUniform1i(14,4);
                }
            }

            //Bind vao
            nPrim->Bind();
            glDrawElements(GL_TRIANGLES, nPrim->iBufferInfo.elementSize, nPrim->iBufferInfo.GLCompType, (void *)0);
            
            glActiveTexture(0);
            glBindTexture(GL_TEXTURE_2D,0);
        }
    }

}

void SceneView::renderer::postdraw()
{
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    size_t tex = mFramebuffer->getTextureID();
    ImGui::Image(reinterpret_cast<void *>(tex), ImVec2{static_cast<float>(mFramebuffer->getWidth()),static_cast<float>(mFramebuffer->getHeight())}, ImVec2{0,1}, ImVec2{1,0});
}

void SceneView::renderer::setColor(glm::vec4 color)
{
    glClearColor(color[0], color[1], color[2], color[3]);
}

void SceneView::renderer::enable(size_t flags)
{
    if((flags&EnableFlags_DEPTHTEST) == EnableFlags_DEPTHTEST){
        glEnable(GL_DEPTH_TEST);
    }
    if((flags&EnableFlags_BLEND_SRC_ALPHA) == EnableFlags_BLEND_SRC_ALPHA){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }  
    if((flags&EnableFlags_BLEND_DST_ALPHA) == EnableFlags_BLEND_DST_ALPHA){
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    }
}

void SceneView::renderer::disable(size_t flags)
{
    if(flags&EnableFlags_DEPTHTEST == EnableFlags_DEPTHTEST){
        glDisable(GL_DEPTH_TEST);
    }
    if(flags&EnableFlags_BLEND_SRC_ALPHA == EnableFlags_BLEND_SRC_ALPHA){
        glDisable(GL_BLEND);
    }  
    if(flags&EnableFlags_BLEND_DST_ALPHA == EnableFlags_BLEND_DST_ALPHA){
        glDisable(GL_BLEND);
    }
}

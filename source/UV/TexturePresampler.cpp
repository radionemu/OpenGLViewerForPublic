#include "TexturePresampler.hpp"

namespace UV
{ 
    TexturePresampler::TexturePresampler()
    {
        this->mShader = std::make_unique<shaders::Shader>();        
        this->mFrameBuffer = std::make_unique<FrameBuffer>();
        this->mVAO = std::make_unique<VertexArray>();
        this->mPOSLayout = std::make_unique<BufferLayout>();
        this->mUVLayout = std::make_unique<BufferLayout>();
        this->mVBO = std::make_unique<VertexBuffer>();
        this->mUBO = std::make_unique<UniformBuffer>();//not use in current build
    }

    TexturePresampler::~TexturePresampler()
    {
    }

    void TexturePresampler::init()
    {
        this->mShader->init();
        this->mShader->LoadVF_GLSL("./shaders/TexturePreSampler/Vertex.vert", "./shaders/TexturePreSampler/Fragment.frag");

        this->mVAO->Bind();
        this->mVBO->Bind();
        this->mVBO->FillData(&quad[0], sizeof(float)*24);
        BufferLayout pos;
        this->mPOSLayout->DirectPush(0, GL_FLOAT, 2, 0);
        this->mUVLayout->DirectPush(1, GL_FLOAT, 2, sizeof(float)*12);
        this->mVAO->AttribBuffer(*mVBO, *mPOSLayout);
        this->mVAO->AttribBuffer(*mVBO, *mUVLayout);
    }
    unsigned int TexturePresampler::createTexturePreview(unsigned int texid)
    {
        this->mShader->useProgram();
        this->mFrameBuffer->generateBuffer(SampleResolution);//init new FBO
        this->mFrameBuffer->Bind();//Bind
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        if(texid != -1){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texid);
            glUniform1i(glGetUniformLocation(mShader->getProgramID(), "TextureSampler"),0);
            this->mVAO->Bind();
            this->mVBO->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        else{
            //create empty texture for texindex [-1]
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        }
        //Deactive
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        //finish
        return mFrameBuffer->getTextureID();
    }
} // namespace UV

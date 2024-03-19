#include "GLVTexture.hpp"
#include "general/texture.hpp"
#include "source/UV/TexturePresampler.hpp"

#include <GL/glew.h>
#include <fx/gltf.h>

namespace GLVGLTF{

    int GLVTexture::mimeType(std::string str)
    {
        int strsize = str.size();
        if(str.substr(strsize-3, 4) == "png") return 0;
        if(str.substr(strsize-3, 4) == "jpg") return 1;
        if(str.substr(strsize-4, 5) == "jpeg") return 1;
        if(str.substr(strsize-3, 4) == "DDS") return 2;
        else return -1;
    }

    void GLVTexture::createbyMIME(fx::gltf::Document *doc)
    {
        auto const & img = doc->images[this->srcIndex];
        if(img.mimeType == "image/png" || mimeType(img.uri) == 0){
            GLVTextureId = LoadPNG(fullPath.string().c_str());
        }
        else if(img.mimeType == "image/jpg" || mimeType(img.uri)== 1){
            GLVTextureId = LoadJPG(fullPath.string().c_str());
        }
        else if(img.mimeType == "image/vnd-ms.dds" || img.mimeType == "image/dds" || mimeType(img.uri)== 2){
            GLVTextureId = LoadDDS(fullPath.string().c_str());
        }
    }

    void GLVTexture::createbyMIME(std::string path)
    {
        printf("%s\n", path.c_str());
        if(mimeType(path) == 0){
            GLVTextureId = LoadPNG(fullPath.string().c_str());
        }
        else if(mimeType(path)== 1){
            GLVTextureId = LoadJPG(fullPath.string().c_str());
        }
        else if(mimeType(path)== 2){
            GLVTextureId = LoadDDS(fullPath.string().c_str());
        }
    }

    GLVTexture::GLVTexture(absFile *absfile, std::int64_t texindex)
    {
        this->DH = (fx::gltf::Document *)absfile->getDocument();
        modelPath = *(std::filesystem::path *)(absfile->getModelPath());
        this->texindex = texindex;
        auto & texture = DH->textures[texindex];
        this->srcIndex = texture.source;
        this->smpIndex = texture.sampler;

        LoadTexture();
    }

    GLVTexture::GLVTexture(std::string imgPath)
    {
        LoadTexture(imgPath);
    }

    GLVTexture::~GLVTexture()
    {
    }

    //try Loading Texture based on model path
    //this function redefine fullpath based on model path if fullpath is not null
    void GLVTexture::LoadTexture()
    {   
        //source
        auto & source = DH->images[this->srcIndex];
        if(!source.IsEmbeddedResource()&&!source.uri.empty()){
            printf("Texture is seperated\n");
            std::filesystem::path rootDir = fx::gltf::detail::GetDocumentRootPath(modelPath);
            this->fullPath = fx::gltf::detail::CreateBufferUriPath(rootDir, DH->images[srcIndex].uri);
            printf("FULL PATH [%s]\n", this->fullPath.string().c_str());
            createbyMIME(DH);
        }
        else{
            if(source.IsEmbeddedResource()){
                printf("EMBEDDED\n");
                std::vector<uint8_t> embeddedData{};
                source.MaterializeData(embeddedData);
                uint32_t BinarySize = static_cast<uint32_t>(embeddedData.size());
                uint8_t const * BinaryData = &embeddedData[0];
                // GLVTextureId = LoadPNG_Mem(BinaryData, BinarySize);
            }else{
                fx::gltf::BufferView const & bufferView = DH->bufferViews[source.bufferView];
                fx::gltf::Buffer const & buffer = DH->buffers[bufferView.buffer];

                uint32_t BinarySize = bufferView.byteLength;
                uint8_t const * BinaryData = &buffer.data[bufferView.byteOffset];

                GLVTextureId = LoadPNG_Mem(BinaryData, BinarySize);
            }
        }

        //previewUV
        UV::TexturePresampler prevSampler;
        prevSampler.init();
        GLVPrevUVID = prevSampler.createTexturePreview(GLVTextureId);
    }
    
    //this function
    void GLVTexture::LoadTexture(std::string userPath)
    {
        this->fullPath = std::filesystem::path(userPath);
        createbyMIME(fullPath.string());
        UV::TexturePresampler prevSampler;
        prevSampler.init();
        GLVPrevUVID = prevSampler.createTexturePreview(GLVTextureId);
    }
}

#pragma once
#include <string>
#include <vector>
#include <fx/gltf.h>
#include "GLVFile.hpp"
#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

namespace fs = std::filesystem;

namespace GLVGLTF{
    class FileManager{
        private:
            fs::path inputPath;
            
            std::vector<fs::path> fList;
            int fListsize = 0;

            //func
            void addFile(fs::path fpath);
            bool isGLTF(fs::path fpath);

            //total minmax
            glm::vec3 mMax = glm::vec3(-9999999);
            glm::vec3 mMin = glm::vec3(9999999);
        public:
            std::vector<GLVFile*> fLists;
            std::vector<GLVTexture *> ExternTextures;

            // FileManager(std::string path);
            bool init(std::string path);
            fx::gltf::Document getDocumentbyFormat();

            void updateTotalMinMax();
            glm::vec3 getmMax(){return mMax;}
            glm::vec3 getmMin(){return mMin;}
            int getSize(){return fListsize;};
            std::vector<GLVFile*>* getfLists(){return &fLists;}
            std::vector<GLVTexture *>* getExternTextures(){return &ExternTextures;}
    };
}


#endif
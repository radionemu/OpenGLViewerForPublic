#include "Filemanager.hpp"
#include "GLVFile.hpp"
#include <fx/gltf.h>

#define FM_GLTF 1
#define FM_GLB 2

namespace fs = std::filesystem;

//TEST COMPLETED
namespace GLVGLTF{
    void FileManager::addFile(fs::path fpath)
    {
        if(isGLTF(fpath)){
            try{
                this->fLists.emplace_back(new GLVFile(fpath));
                this->fListsize++;
                printf("ADDED FILE : %s || FLIST SIZE : %d\n", fpath.string().c_str(), this->fListsize);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }

    bool FileManager::isGLTF(fs::path fpath)
    {
        if(fpath.extension() == ".gltf" || fpath.extension() == ".glb")return true;
        return false;
    }

    bool FileManager::init(std::string path)
    {
        this->inputPath = fs::path(path);
        if(!fs::exists(inputPath)){
            fprintf(stderr, "The File or Directory does not exist.\n");
            return false;
        }
        if(fs::is_regular_file(inputPath)){
            //single Files
            addFile(inputPath);
        }else if(fs::is_directory(inputPath)){
            //Find all Files in Directory
            for(const fs::directory_entry& entry : fs::recursive_directory_iterator(inputPath)){
                addFile(entry.path());
            }
        }
        //update boundingBox
        updateTotalMinMax();

        return true;
    }

    fx::gltf::Document FileManager::getDocumentbyFormat()
    {
        

        return fx::gltf::Document();
    }
    
    void FileManager::updateTotalMinMax()
    {
        for(GLVFile* glFile : fLists){
            glm::vec3 nMax = glFile->getmMax();
            glm::vec3 nMin = glFile->getmMin();
            printf("\tnMAX[%f][%f][%f] || nMIN[%f][%f][%f]\n", nMax[0],nMax[1],nMax[2],nMin[0],nMin[1],nMin[2]);
            for(size_t i=0; i<3; i++){
                mMax[i] = mMax[i] < nMax[i] ? nMax[i] : mMax[i];
                mMin[i] = mMin[i] > nMin[i] ? nMin[i] : mMin[i];
            }
        }
        printf("\t\tMAX[%f][%f][%f] || MIN[%f][%f][%f]\n", mMax[0],mMax[1],mMax[2],mMin[0],mMin[1],mMin[2]);
    }
}

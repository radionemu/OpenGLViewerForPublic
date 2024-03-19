#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <wtypes.h>

#include <chrono>

#include <GL/glew.h>
#include <algorithm>
#include <cmath>
#include <cstring>

//WIN32
#include <windows.h>
#include <GLVWin32/GLVWindow.hpp>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//IMGUI
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>

//GLTF
#include <fx/gltf.h>

#include <general/loadshader.hpp>
#include <general/objloader.hpp>
#include <general/mtlloader.hpp>
#include <general/texture.hpp>
#include <general/resolution.hpp>
#include <general/GLVBuffer/VertexBuffer.hpp>
#include <general/GLVVertexArray/VertexArray.hpp>
#include <general/GLVVertexArray/BufferLayout.hpp>
#include <general/GLVBuffer/UniformBuffer.hpp>

#include <GLVgltf/GLVMesh.h>

//NEW GLV SERIES
#include <general/GLVFileLoad/FileManager.hpp>
#include <general/GLVFileLoad/GLVFile.hpp>
#include <GLVgltf/GLVScene.hpp>
#include <GLVgltf/GLVNode.hpp>
#include <GLVgltf/GLVMeshes.hpp>
#include <GLVgltf/GLVPrimitive.hpp>
#include <GLVgltf/GLVMaterial.hpp>
#include <GLVgltf/GLVTexture.hpp>

#include "source/WindowManager/WinManager.hpp"

#define CAMSPD 0.05
#define SENSITIVITY 1
#define TEXNULL 4294967295
// #define IMGUILOAD
// #define USEGLUT

using namespace std;
   int z = 0;
//temp
glm::mat4 transformCam = glm::mat4(1.0);
static int legacyx;
static int legacyy;
int specialkey = 0;
int midbutton = 0;
int deltax, deltay;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
bool isclick = false;
size_t smeshcount = 0;

//VertexBuffer
VertexBuffer **vbo;
BufferLayout layouts;
vector<VertexArray> vao;


GLVMesh **meshes;
size_t meshSize = 0;

//pbrColor
GLuint PBRbaseColor;

//MVP
GLuint MVPID;

//Buffers
GLuint programID;
GLuint VertexArrayID;
GLuint Buffers[3];
GLuint ibo;

GLuint PrjMID;

GLuint vtxPosID;
GLuint vtxUVID;
GLuint vtxNormID;
GLuint TextureID;

vector<obj> objCollection;
vector<mtl> mtlCollection;
vector<GLuint> textureCollection;

GLuint TCamID;

//WIN32 X GLEW

static void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError(){
    while(GLenum error = glGetError()){
        cout << "[OpenGL Error] : " << error << endl;
    }
}

int main(int argc, char **argv)
{   
    std::unique_ptr<WindowManager::WinManager> winmgr = std::make_unique<WindowManager::WinManager>();
    winmgr->init(1280,720,"./DamagedHelmet.gltf"); 
    while(true){
        winmgr->render();
    }
    return 1;
}
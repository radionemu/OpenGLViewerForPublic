#pragma once
#include <source/incls.h>

namespace SceneView{
    class Trackball
    {
    private:
        glm::vec3 lastpos = glm::vec3(0.0f);
        glm::vec3 curpos = glm::vec3(0.0f);
    public:
        Trackball(/* args */);
        ~Trackball();

        void init();
        void ptov(int mouseX, int mouseY,int screenW, int screenH, float* vec);
        void mouseDown(int mouseX, int mouseY,int screenW, int screenH);
        void mouseMove(int mouseX, int mouseY,int screenW, int screenH);
        void mouseUp();
    };
}
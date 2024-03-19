#pragma once
#ifndef BUFFER_LAYOUT_HPP
#define BUFFER_LAYOUT_HPP

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <vector>

static void GLClearError2(){
    while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError2(){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] : " << error << std::endl;
    }
}

typedef struct VertexPointer{
    uint64_t count;
    uint64_t vptype;
    uint64_t normalize;
    uint64_t LayoutID;
    uint64_t offset;
}VertexPointer;

//store the Information about glEnableVertexAttribArray() + glVertexAttribPointer()
class BufferLayout{
    private:
        std::vector<VertexPointer> VertElement;
        uint64_t stride = 0;
    public:
        BufferLayout();

        std::vector<VertexPointer> getElement(){return VertElement;}
        //test
        void Push(unsigned int programID, const char *Layout, GLuint gltype, uint64_t pcount, uint64_t offset){
            GLClearError2();
            GLuint tLID = glGetAttribLocation(programID, Layout);
            GLCheckError2();
            VertexPointer temp = {pcount, gltype, GL_FALSE, tLID, offset};
            VertElement.push_back(temp);
            stride += sizeof(float)*pcount;
            //printf("layout push done\n");
        }

        void DirectPush(unsigned int layoutID, GLuint gltype, uint64_t pcount, uint64_t offset){
            VertexPointer temp = {pcount, gltype, GL_FALSE, layoutID, offset};
            VertElement.push_back(temp);
            stride += sizeof(float)*pcount;
        }

        uint64_t Getstride(){
            return stride;
        }

        void Clear();
};



#endif
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<vector>
#include<iostream>
#include<fstream>

#include<GL/glew.h>

#include "loadshader.hpp"

using namespace std;

namespace shaders
{
    void Shader::init()
    {
        this->mProgramID = glCreateProgram();
    }

    void Shader::useProgram()
    {
        glUseProgram(this->mProgramID);
    }

    void Shader::deleteProgram()
    {
        glDeleteProgram(this->mProgramID);
    }

    void Shader::load()
    {
        glUseProgram(this->mProgramID);
        unsigned int vsID = LoadGLSLID(GL_VERTEX_SHADER, "VertexShader.vert");
        unsigned int fsID = LoadGLSLID(GL_FRAGMENT_SHADER, "FragmentShader.frag");
        glAttachShader(this->mProgramID, vsID);
        glAttachShader(this->mProgramID, fsID);

        glLinkProgram(this->mProgramID);
        glValidateProgram(this->mProgramID);

        glDeleteShader(vsID);
        glDeleteShader(fsID);

        // LoadShaderGLSL("VertexShader.vert", "FragmentShader.frag");
    }

    void Shader::LoadVF_GLSL(const char *vertex_file_path, const char *fragment_file_path)
    {
        glUseProgram(this->mProgramID);
        unsigned int vsID = LoadGLSLID(GL_VERTEX_SHADER, vertex_file_path);
        unsigned int fsID = LoadGLSLID(GL_FRAGMENT_SHADER, fragment_file_path);
        glAttachShader(this->mProgramID, vsID);
        glAttachShader(this->mProgramID, fsID);

        glLinkProgram(this->mProgramID);
        glValidateProgram(this->mProgramID);

        glDeleteShader(vsID);
        glDeleteShader(fsID);
    }

    GLuint Shader::LoadGLSLID(GLuint _glewCreateShader, const char *shader_path)
    {
        GLuint ShaderID = glCreateShader(_glewCreateShader);
        GLint Result = GL_FALSE;
        int InfoLogLength;

        //Read the shader code from the file
        string ShaderCode;
        ifstream ShaderStream(shader_path, ios::in);
        if(!ShaderStream.is_open()){
            printf("Shader : [%s] Open failed\n", shader_path);
        }
        if(ShaderStream.is_open())
        {
            printf("Shader : [%s] Opened\n", shader_path);
            string Line = "";
            while(getline(ShaderStream, Line)){
                ShaderCode += "\n" + Line;
            }

            ShaderStream.close();
        }

        //Compile Shader
        printf("Compiling shader : %s\n", shader_path);
        char const* SourcePointer = ShaderCode.c_str();
        glShaderSource(ShaderID, 1, &SourcePointer, NULL);
        glCompileShader(ShaderID);

        //Check Shader
        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(InfoLogLength != 0){
            vector<char> ShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
            fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
        }

        return ShaderID;
    }

    GLuint Shader::LoadSPIRVID(GLuint _glewCreateShader, const char *shader_path)
    {
        //load SPIR-V binary code
        uint8_t* shader_buf;
        size_t shader_buf_len;
        FILE *fp = fopen64(shader_path, "rb");

        GLint compiled = 0;
        int InfoLogLength = 0;

        GLuint ShaderID = glCreateShader(GL_VERTEX_SHADER);

        if(fp == NULL){
            printf("SPIR-V Shader  : [%s] load failed\n", shader_path);
        }else{
            fseek(fp, 0, SEEK_END);
            shader_buf_len = ftell(fp);
            rewind(fp);
            if(!(shader_buf = (uint8_t *)malloc(shader_buf_len+1))){
                printf("SPIR-V Shader  : [%s] Memory Allocate failed\n", shader_path);
            }
            if(fread(shader_buf, 1, shader_buf_len, fp) < shader_buf_len){
                printf("Vertex Shader SPIR-V : [%s] Shader load to Memory failed\n", shader_path);
            }
            fclose(fp);
            printf("Vertex Shader SPIR-V : [%s] Loaded on memory\n", shader_path);
        }
        
        glShaderBinary(1, &ShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, shader_buf, shader_buf_len);
        glSpecializeShader(ShaderID, "main", 0,0,0);
        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compiled);
        glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(!compiled){
            if(InfoLogLength != 0){
                vector<char> ShaderErrorMessage(InfoLogLength);
                glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
                fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
            }
        }
        else{
            printf("SPIR-V Shader : [%s] Compile Verified\n", shader_path);
        }

        return ShaderID;
    }

    //return programID
    GLuint Shader::LoadShaderGLSL(const char* vertex_file_path, const char* fragment_file_path){
        //create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        GLint Result = GL_FALSE;
        int InfoLogLength;

        //Read the vertex shader code from the file
        string VertexShaderCode;
        ifstream VertexShaderStream(vertex_file_path, ios::in);
        if(!VertexShaderStream.is_open()){
            printf("Vertex Shader : [%s] Open failed\n", vertex_file_path);
        }
        if(VertexShaderStream.is_open())
        {
            printf("Vertex Shader : [%s] Opened\n", vertex_file_path);
            string Line = "";
            while(getline(VertexShaderStream, Line)){
                VertexShaderCode += "\n" + Line;
            }

            VertexShaderStream.close();
        }

        //Compile Vertex Shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const* VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        //Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(InfoLogLength != 0){
            vector<char> VertexShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
        }

        //Read the fragment shader code from the file
        string FragmentShaderCode;
        ifstream FragmentShaderStream(fragment_file_path, ios::in);
        if(FragmentShaderStream.is_open())
        {
            printf("FragmentShader : [%s] Opened\n", fragment_file_path);
            string Line = "";
            while(getline(FragmentShaderStream, Line)){
                FragmentShaderCode += "\n" + Line;
            }
            FragmentShaderStream.close();
        }

        //Compile Fragment Shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const* FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        //Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(InfoLogLength != 0){
            vector<char> FragmentShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
        }

        //Link the program
        fprintf(stdout, "Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(InfoLogLength != 0){
            vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
        }

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        this->mProgramID = ProgramID;
        return ProgramID;
    }

    //return programID
    GLuint Shader::LoadShaderSPIRV(const char* vertex_file_path, const char* fragment_file_path){
        //load SPIR-V binary code
        uint8_t* vs_buf;
        size_t vs_buf_len;
        FILE *fp = fopen64(vertex_file_path, "rb");
        if(fp == NULL){
            printf("Vertex Shader SPIR-V : [%s] load failed\n", vertex_file_path);
        }else{
            fseek(fp, 0, SEEK_END);
            vs_buf_len = ftell(fp);
            rewind(fp);
            if(!(vs_buf = (uint8_t *)malloc(vs_buf_len+1))){
                printf("Vertex Shader SPIR-V : [%s] Memory Allocate failed\n", vertex_file_path);
            }
            if(fread(vs_buf, 1, vs_buf_len, fp) < vs_buf_len){
                printf("Vertex Shader SPIR-V : [%s] Shader load to Memory failed\n", vertex_file_path);
            }
            fclose(fp);
            printf("Vertex Shader SPIR-V : [%s] Loaded on memory\n", vertex_file_path);
        }

        uint8_t* fs_buf;
        size_t fs_buf_len;
        FILE *fp2 = fopen64(fragment_file_path, "rb");
        if(fp2 == NULL){
            printf("Fragment Shader SPIR-V : [%s] load failed\n", fragment_file_path);
        }else{
            fseek(fp2, 0, SEEK_END);
            fs_buf_len = ftell(fp2);
            rewind(fp2);
            if(!(fs_buf = (uint8_t *)malloc(fs_buf_len+1))){
                printf("Fragment Shader SPIR-V : [%s] Memory Allocate failed\n", fragment_file_path);
            }
            if(fread(fs_buf, 1, fs_buf_len, fp2) < fs_buf_len){
                printf("Fragment Shader SPIR-V : [%s] Shader load to Memory failed\n", fragment_file_path);
            }
            printf("Fragment Shader SPIR-V : [%s] Loaded on memory\n", fragment_file_path);
        }
        fclose(fp2);

        GLuint ProgramID = glCreateProgram();
        int compiled = 0;
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderBinary(1, &VertexShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, vs_buf, vs_buf_len);
        glSpecializeShader(VertexShaderID, "main", 0,0,0);
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &compiled);
        if(compiled){
            glAttachShader(ProgramID, VertexShaderID);
            printf("Vertex Shader SPIR-V : [%s] Compile Verified\n", vertex_file_path);
        }
        compiled = 0;
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderBinary(1, &FragmentShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, fs_buf, fs_buf_len);
        glSpecializeShader(FragmentShaderID, "main", 0,0,0);
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &compiled);
        if(compiled){
            glAttachShader(ProgramID, FragmentShaderID);
            printf("Fragment Shader SPIR-V : [%s] Compile Verified\n", fragment_file_path);
        }
        free(vs_buf);
        free(fs_buf);
        printf("Link Program\n");
        glLinkProgram(ProgramID);

        // Check the program
        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(InfoLogLength != 0){
            vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
        }

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }
} // namespace shaders


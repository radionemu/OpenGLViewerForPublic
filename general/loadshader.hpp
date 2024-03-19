#pragma once
#ifndef LOADSHADER_HPP
#define LOADSHADER_HPP

namespace shaders{

    class Shader
    {
    private:
        GLuint mProgramID; //Program's ID

    public:

    void init();

    void useProgram();

    void deleteProgram();

    void load();

    void LoadVF_GLSL(const char* vertex_file_path, const char* fragment_file_path);

    GLuint LoadGLSLID(GLuint _glewCreateShader, const char* shader_path);

    GLuint LoadSPIRVID(GLuint _glewCreateShader, const char* shader_path);

    GLuint LoadShaderGLSL(const char* vertex_file_path, const char* fragment_file_path);

    GLuint LoadShaderSPIRV(const char* vertex_file_path, const char* fragment_file_path);        

    GLuint getProgramID(){return mProgramID;};

    };

}


#endif
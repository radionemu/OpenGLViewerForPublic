#ifndef MVP_HPP
#define MVP_HPP

#include<glm/glm.hpp>

class GL_MVP{
private:
    //scene position range
    glm::vec3 maxPosition = glm::vec3(0.0f);
    glm::vec3 minPosition = glm::vec3(0.0f);
    glm::vec3 camPos = glm::vec3(0.0f);
    glm::vec3 centre = glm::vec3(0.0f);
    glm::vec3 dir = glm::vec3(0.0f);
    glm::vec3 camR = glm::vec3(0.0f);
    glm::vec3 camU = glm::vec3(0.0f,1.0f,0.0f);
    glm::vec3 initR = glm::vec3(0.0f);
    glm::vec3 initU = glm::vec3(0.0f);
    //For Models
    glm::mat4 Mscale = glm::mat4(1.0f);
    glm::mat4 Mrotation = glm::mat4(1.0f);
    glm::mat4 MTempRot = glm::mat4(1.0f);
    glm::mat4 Mtranform = glm::mat4(1.0f);
    //For Views
    glm::mat4 VRotation = glm::mat4(1.0f);
    glm::mat4 VTempRot = glm::mat4(1.0f);
    glm::mat4 VTransform = glm::mat4(1.0f);
    glm::mat4 VCamTemp = glm::mat4(1.0f);
    //For Projections
    float FoV = 45.0f; //based on degree
    float Fnear = 0.1f;
    float Ffar = 10000.0f;
    //final matrices
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 View = glm::mat4(1.0f);
    glm::mat4 Projection = glm::mat4(1.0f);
    glm::mat4 MVP = glm::mat4(1.0f);
public:
    GL_MVP();

    glm::mat4 getModelMatrix();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getMVP();

    void UpdateMVP();
    
    void UpdateProjection(float iFoV, float near, float far);
        void UpdateProjection(float iFoV){UpdateProjection(iFoV, Fnear, Ffar);}
        void UpdateProjection(float inear, float ifar){UpdateProjection(FoV, inear, ifar);}
    
    void UpdateView();
        void TranslateView(glm::vec3 pos);
        void TranslateView(float x, float y, float z);
        void RotateView(glm::vec3 rot);
        void temp2RotView();
        void InitView(glm::vec3 rot);

        void translateCamera(glm::vec3 pos);
        void rotateCamera(glm::vec3 delta);
        void zoomCamera(glm::vec3 delta);
        void mzoomCamera(glm::vec3 delta);
        void eventCamera();
        void acceptCamera();
    void UpdateModel();
        void TranslateModel(glm::vec3 mpos);
        void RotateModel(glm::vec3 mrot);
        void temp2RotModel();
        void ScaleModel(glm::vec3 mscl);


    glm::mat4 getTranslateCam();
    void ClickCamera(int button, int state, int x, int y);

    void setMaxMin(glm::vec3 &maxp, glm::vec3 &minp){
        maxPosition = maxp; minPosition = minp;
    }

    void getMaxMin(glm::vec3 &maxp, glm::vec3 &minp){
        maxp = maxPosition; minp = minPosition;
    }
};



#endif
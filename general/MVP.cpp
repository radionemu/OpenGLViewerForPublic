#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

#include <stdio.h>

#include "resolution.hpp"
#include "MVP.hpp"

//provide result MVP matrix
GL_Resolution gl_resolution = GL_Resolution::getResInstance();

//for Camera Control
//glm::mat4 transformCam = glm::mat4(1.0);
//glm::vec3 Viewposition = glm::vec3(0,0,5);
//float ViewAngleH = 3.14f;
//float ViewAngleV = 0;

//Proejction Setting
//float initialFoV = 45.0f;

GL_MVP::GL_MVP(){
    UpdateModel();
    UpdateView();
    UpdateProjection(FoV,Fnear,Ffar);
    UpdateMVP();
}

glm::mat4 GL_MVP::getViewMatrix(){
    return View;
}

glm::mat4 GL_MVP::getProjectionMatrix(){
    return Projection;
}

//must update MVP before call this function if you want most recent matrix
glm::mat4 GL_MVP::getMVP(){
    return MVP;
}

void GL_MVP::UpdateMVP(){
    MVP = Projection * View * Model;
}

//you cannot change ratio
void GL_MVP::UpdateProjection(float iFoV, float inear, float ifar){
    glm::vec2 windowRes = gl_resolution.GetWindowResolution();
    Projection = glm::perspective(glm::radians(iFoV), windowRes.x/windowRes.y, inear, ifar);
}

void GL_MVP::UpdateView()
{
    View =  VTransform * VTempRot * VRotation * VCamTemp;
}

void GL_MVP::TranslateView(glm::vec3 pos)
{
    VTransform = glm::translate(VTransform, pos);
    UpdateView();
}

void GL_MVP::TranslateView(float x, float y, float z)
{
    TranslateView(glm::vec3(x,y,z));
}

void GL_MVP::RotateView(glm::vec3 rot)
{
    VTempRot = glm::rotate(VTempRot, glm::radians(rot.x), glm::vec3(0.f,1.f,0.f));
    VTempRot = glm::rotate(VTempRot, glm::radians(rot.y), glm::vec3(1.f,0.f,0.f));
    UpdateView();
}

void GL_MVP::temp2RotView()
{
    VRotation = VTempRot * VRotation * VCamTemp;

    VTempRot = glm::mat4(1.0f);
    VCamTemp = glm::mat4(1.0f);
    UpdateView();
}


void GL_MVP::InitView(glm::vec3 rot)
{
    centre = glm::vec3((maxPosition.x+minPosition.x)/2.0f,(maxPosition.y+minPosition.y)/2.0f,(maxPosition.z+minPosition.z)/2.0f);//object center
    float maxpos = 0.0f;
    for(int i=0;i<3; i++){
        maxpos = glm::max(abs(maxPosition[i]-centre[i]),maxpos);
    }
    
    TranslateModel(glm::vec3(-centre.x,-centre.y,-centre.z));
    camPos = glm::vec3(maxpos*1.25f);//cam position

    printf("%f %f %f\n", centre.x, centre.y, centre.z);
    printf("%f %f %f\n", camPos.x, camPos.y, camPos.z);

    VRotation = glm::lookAt(
        camPos, // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );//VRotation Change
    UpdateView();
    //Later
}

void GL_MVP::translateCamera(glm::vec3 pos)
{
    dir = camPos - centre;
    camR = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f), dir));
    camU = glm::cross(dir, camR);
    centre = centre + pos.x * camR + pos.y * camU;
    camPos = camPos + pos.x * camR + pos.y * camU;
    VRotation = glm::lookAt(camPos, centre, glm::vec3(0,1,0));
    UpdateView();
}

void GL_MVP::rotateCamera(glm::vec3 delta)
{
    VCamTemp = glm::rotate(VCamTemp, glm::radians(delta.x), glm::vec3(0.f,1.f,0.f));
    VCamTemp = glm::rotate(VCamTemp, glm::radians(delta.y), glm::vec3(1.f,0.f,0.f));
    UpdateView();
}

void GL_MVP::zoomCamera(glm::vec3 delta)
{
    dir = camPos - centre;
    dir = (1+delta.z)*dir;
    camPos = centre + dir;
    VRotation = glm::lookAt(camPos, centre, glm::vec3(0,1,0));
    UpdateView();
}

void GL_MVP::mzoomCamera(glm::vec3 delta)
{
    dir = camPos - centre;
    dir = (delta.z)*glm::normalize(dir)+dir;
    camPos = centre + dir;
    VRotation = glm::lookAt(camPos, centre, glm::vec3(0,1,0));
    UpdateView();
}

void GL_MVP::eventCamera()
{
    dir = camPos - centre;
    initR = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f), dir));
    initU = glm::cross(dir, initR);
}

void GL_MVP::UpdateModel()
{
    Model = MTempRot * Mtranform * Mrotation * Mscale;
}

void GL_MVP::TranslateModel(glm::vec3 mpos)
{
    Mtranform = glm::translate(Mtranform, mpos);
    UpdateModel();
}

void GL_MVP::RotateModel(glm::vec3 mrot)
{
    MTempRot = glm::rotate(MTempRot, glm::radians(mrot.x), glm::vec3(0.f,1.f,0.f));
    MTempRot = glm::rotate(MTempRot, glm::radians(mrot.y), glm::vec3(1.f,0.f,0.f));
    UpdateModel();
}

void GL_MVP::temp2RotModel(){
    Mrotation = MTempRot * Mrotation;
    MTempRot = glm::mat4(1.0f);
    UpdateModel();
}

void GL_MVP::ScaleModel(glm::vec3 mscl)
{
    Mscale = glm::scale(Mscale, mscl);
    UpdateModel();
}

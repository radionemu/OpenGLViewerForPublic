#include "camera.hpp"
#include <source/incls.h>

namespace SceneView{

camera::camera()
{
}

camera::~camera()
{
}

void camera::init(glm::vec3 max, glm::vec3 min)
{
    centre = glm::vec3((max.x+min.x)/2.0f,(max.y+min.y)/2.0f,(max.z+min.z)/2.0f);//object center
    float maxpos = 0.0f;
    for(int i=0;i<3; i++){
        maxpos = glm::max(abs(max[i]-centre[i]),maxpos);
    }
    camPos = glm::vec3(centre.x+maxpos*1.5f,centre.y,centre.z+maxpos*1.5f);//cam position

    mVMat = mVinit = glm::lookAt(
        camPos, // Camera is at (4,3,3), in World Space
        centre, // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
}

void camera::initFront(glm::vec3 max, glm::vec3 min)
{
    centre = glm::vec3((max.x+min.x)/2.0f,(max.y+min.y)/2.0f,(max.z+min.z)/2.0f);//object center
    float maxpos = 0.0f;
    float dx = (max.x-min.x)/2.0f;
    float dy = (max.y-min.y)/2.0f;
    float dz = (max.z-min.z)/2.0f;
    camPos = glm::vec3(centre.x,centre.y,centre.z+(dz+dx*2));
    mVMat = mVinit = glm::lookAt(
        camPos, // Camera is at (4,3,3), in World Space
        centre, // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
}

void camera::update()
{
    
}

void camera::updateMVP()
{
    mMVP = mPMat * mVMat * mWMat * mMMat;
}

void camera::updateProjection(float mWidth, float mHeight)
{
    mPMat = glm::perspective(glm::radians(mFov), mWidth/mHeight, mNear, mFar);
}

void camera::updateView()
{
    mVMat = mVFPcamTransform * mVFPcamRotation * mVinit;
}

void camera::updateViewFPRot()
{
    mVFPcamRotation = mVFPRotDelta * mVFPLrot;
    updateView();
}



void camera::rotateViewFPRot(glm::vec3 rot)
{
    mVFPRotDelta = glm::rotate(mVFPRotDelta, glm::radians(rot.x), glm::vec3(0.f,1.f,0.f));
    mVFPRotDelta = glm::rotate(mVFPRotDelta, glm::radians(rot.y), glm::vec3(1.f,0.f,0.f));
    updateViewFPRot();
}



void camera::updateWorld()
{
    mWMat = mWTransform * mWRotation;
}

void camera::rotateWorldRot(glm::vec3 axis, float angle)
{
    mWRotTemp = glm::rotate(mWRotTemp, angle, axis);
    updateWorldRot();
}

void camera::acceptWorldRot()
{
    mWRotLegacy = mWRotTemp * mWRotLegacy;
    mWRotTemp = glm::mat4(1.f);
    updateWorldRot();
}

void camera::rorateModelRot(glm::vec3 axis, float angle)
{
    mMMat = glm::rotate(mMMat, angle, axis);
}

void camera::updateWorldRot()
{
    mWRotation = mWRotTemp * mWRotLegacy;
    updateWorld();
}

void camera::acceptViewFPRot()
{
    mVFPLrot = mVFPRotDelta * mVFPLrot;
    mVFPRotDelta = glm::mat4(1.f);
    updateViewFPRot();
}

void camera::translateViewFPTransform(glm::vec3 pos)
{
    mVFPcamTransform = glm::translate(mVFPcamTransform, pos);
    updateView();
}

glm::mat4 camera::getMVP()
{
    return mMVP;
}

}

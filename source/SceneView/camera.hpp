#pragma once
#include <source/incls.h>

namespace SceneView
{
    class camera
    {
    private:
        glm::vec3 camPos;
        glm::vec3 centre;

        //Final MVPmat
        glm::mat4 mMVP = glm::mat4(1.0f);

        //Projections
        glm::mat4 mPMat = glm::mat4(1.0f);
            float mFov = 45.0f;
            float mNear = 0.001f;
            float mFar = 10000.0f;

        //Views
        glm::mat4 mVMat = glm::mat4(1.0f);
            glm::mat4 mVinit = glm::mat4(1.0f);
            glm::mat4 mVFPcamTransform = glm::mat4(1.0f);
            glm::mat4 mVFPcamRotation = glm::mat4(1.0f);
                glm::mat4 mVFPLrot = glm::mat4(1.0f);
                glm::mat4 mVFPRotDelta = glm::mat4(1.0f);
        
        //Worlds
        glm::mat4 mWMat = glm::mat4(1.0f);
            glm::mat4 mWRotation = glm::mat4(1.0f);
            glm::mat4 mWRotLegacy = glm::mat4(1.0f);
            glm::mat4 mWRotTemp = glm::mat4(1.0f);
            glm::mat4 mWTransform = glm::mat4(1.0f);

        //Models
        glm::mat4 mMMat = glm::mat4(1.0f);//this is temp mat
            glm::mat4 mMMaty = glm::mat4(1.f);
            glm::mat4 mMMatx = glm::mat4(1.f);


    public:
        camera(/* args */);
        ~camera();

        void init(glm::vec3 max, glm::vec3 min);
        void initFront(glm::vec3 max, glm::vec3 min);

        void update();

        void updateMVP();
    
        void updateProjection(float mWidth, float mHeight);

        void updateView();
        void updateViewFPRot();
        void translateViewFPTransform(glm::vec3 rot);
        void rotateViewFPRot(glm::vec3 rot);
        void acceptViewFPRot();

        void updateWorld();
        void updateWorldRot();
        void rotateWorldRot(glm::vec3 axis, float angle);
        void acceptWorldRot();

        void rorateModelRot(glm::vec3 axis, float angle);

        glm::mat4 getMVP();
        glm::mat4 getView(){return mVMat;}
        glm::mat4 getProj(){return mPMat;}
        glm::mat4 getWorld(){return mWMat;}
        glm::mat4 getModel(){return mMMat;}
        glm::vec3 getcamPos(){return camPos;}
    };

} // namespace Scene

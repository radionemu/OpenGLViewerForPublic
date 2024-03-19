#include "Trackball.hpp"

namespace SceneView
{

    Trackball::Trackball()
    {
    }

    Trackball::~Trackball()
    {
    }

    void Trackball::init()
    {
    }

    void Trackball::ptov(int mouseX, int mouseY, int screenW, int screenH, float * vec)
    {
        vec[0]=(2.0f*mouseX-screenW)/screenW;//normalized x
        vec[1]=(screenH-2.0f*mouseY)/screenH;//normalized y
        float d = glm::sqrt(vec[0]*vec[0]+vec[1]*vec[1]);//distance from (0,0)
        vec[2]=glm::cos((glm::pi<double>()/2.0f)*((d<1.0f)?d:1.0));
        float a = 1.0f/glm::sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
        vec[0]*=a; vec[1]*=a; vec[2]*=a;
    }

    void Trackball::mouseMove(int mouseX, int mouseY, int screenW, int screenH)
    {
    }

} // namespace SceneView


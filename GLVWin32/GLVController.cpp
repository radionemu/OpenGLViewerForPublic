#include "GLVController.hpp"

GLVController::GLVController()
{
    m_display = new GLVWindow();
}

GLVController::~GLVController()
{
    delete m_display;
}

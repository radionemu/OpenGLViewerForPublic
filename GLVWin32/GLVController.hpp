#include "GLVWindow.hpp"

class GLVController{
    public:
        GLVController();
        ~GLVController();

        void run();
        void quit();
    private:
        bool m_isRunning = true;
        GLVWindow* m_display = nullptr;
};
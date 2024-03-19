#include "source/incls.h"

namespace Resources
{
    class FileDialogManager
    {
    private:
        /* data */
    public:
        FileDialogManager(/* args */);
        ~FileDialogManager();
    
        void init();
        std::string OpenFile();
        std::string SaveFile();
    };
} // namespace Resources

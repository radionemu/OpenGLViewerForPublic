#pragma once
#include <source/incls.h>

namespace UIComponent
{
    class absInspector{
        public:
            virtual std::vector<std::pair<unsigned int, unsigned int>>* getPreviewUVs()=0;
            virtual int binarySearch(unsigned int texID)=0;
    };
} // namespace UIComponent



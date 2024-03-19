#pragma once
#include "source/incls.h"
#include "general/loadshader.hpp"
#include "source/SceneView/camera.hpp"
#include "general/GLVFileLoad/FileManager.hpp"
#include "general/GLVBuffer/FrameBuffer.hpp"
#include "source/SceneView/renderer.hpp"

#include "general/GLVVertexArray/VertexArray.hpp"
#include "general/GLVVertexArray/BufferLayout.hpp"
#include "general/GLVBuffer/VertexBuffer.hpp"
#include "general/GLVBuffer/UniformBuffer.hpp"

namespace UIComponent{
    class Scene : public SceneView::renderer
    {
        private:
            //ScreenSize
                glm::vec2 mPanelSize;
                std::pair<int, int> mScenePixelSize;
                glm::vec2 mCustomSize;

            //Cubemap
                unsigned int cubemap;
                float skyboxVertices[108] = {
                    // positions          
                    -1.0f,  1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                    -1.0f,  1.0f, -1.0f,
                    1.0f,  1.0f, -1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                    1.0f, -1.0f,  1.0f
                };
                std::unique_ptr<VertexArray> skyboxVAO;
                std::unique_ptr<VertexBuffer> skyboxVBO;
                std::unique_ptr<UniformBuffer> skyboxUBO;
                std::unique_ptr<shaders::Shader> skyboxShader;
        public:
        Scene();
        ~Scene();

        void init(std::shared_ptr<GLVGLTF::FileManager> fManager);
        void render();

        void onResize(size_t width, size_t height);
        void onMouseDown(unsigned int MSG, int X, int Y) override;
        void onMouseClick() override;
        void onMouseUp(unsigned int MSG, int X, int Y) override;
        void onMouseMove(int X, int Y) override;
        void onMouseWheel(int Z) override;
    };
}
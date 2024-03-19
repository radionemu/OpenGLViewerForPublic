#include "imgui_context.hpp"
#include "source/incls.h"
#include "source/resource/fonts/Rubik-Regular.hpp"

GLVrender::UIContext::~UIContext()
{
    //Delete IMGUI context
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool GLVrender::UIContext::init(WindowManager::window *absWind)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
            //fonts
        io.Fonts->AddFontFromMemoryTTF(Rubik_Regular, sizeof(Rubik_Regular), 16.0f);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_InitForOpenGL(absWind->getWndHandler());
    ImGui_ImplOpenGL3_Init();//parameter:glsl version
    return true;
}

void GLVrender::UIContext::CreateDocker()
{
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //Docking Environment
        ImGuiWindowFlags winFlag;
        winFlag |= ImGuiWindowFlags_NoDocking;
        winFlag |= ImGuiWindowFlags_NoTitleBar;
        winFlag |= ImGuiWindowFlags_NoCollapse;
        winFlag |= ImGuiWindowFlags_NoResize;
        winFlag |= ImGuiWindowFlags_NoMove;
        winFlag |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        winFlag |= ImGuiWindowFlags_NoNavFocus;
        winFlag |= ImGuiWindowFlags_NoBackground;

        ImGuiViewport* vPort = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vPort->Pos);
        ImGui::SetNextWindowSize(vPort->Size);
        ImGui::SetNextWindowViewport(vPort->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockerSpace", nullptr, winFlag);
        ImGui::PopStyleVar(3);//???

        ImGuiID dockerId = ImGui::GetID("DockerSpace");

        ImGui::DockSpace(dockerId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode|ImGuiDockNodeFlags_AutoHideTabBar);
        ImGui::End();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            // ImGui::ShowDemoWindow(&show_demo_window);
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::ShowDemoWindow(&show_demo_window);

            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
}

void GLVrender::UIContext::RenderFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

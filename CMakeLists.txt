cmake_minimum_required(VERSION 3.10)
project(GLV)

set(CMAKE_CXX_STANDARD 17)
set(OPENGL_PROFILE GLCORE)

set(SRC_DIR ./source)

set(INPUT_DIR ${SRC_DIR}/Input)
set(INPUT_SRC ${INPUT_DIR}/input.hpp)

set(SCENE_DIR ${SRC_DIR}/SceneView)
set(SCENE_SRC
    ${SCENE_DIR}/camera.cpp
    ${SCENE_DIR}/renderer.cpp
    ${SCENE_DIR}/trackball.cpp)

set(CTX_DIR ${SRC_DIR}/Context)
set(CTX_SRC
    ${CTX_DIR}/abstract_context.hpp
    ${CTX_DIR}/imgui_context.cpp
    ${CTX_DIR}/render_context.cpp)

set(UICOMP_DIR ${SRC_DIR}/UIComponent)
set(UICOMP_SRC
    ${UICOMP_DIR}/Hierarchy.cpp
    ${UICOMP_DIR}/Inspector.cpp
    ${UICOMP_DIR}/absInspec.hpp
    ${UICOMP_DIR}/Scene.cpp)    

set(WINMGR_DIR ${SRC_DIR}/WindowManager)
set(WINMGR_SRC
    ${WINMGR_DIR}/win.hpp
    ${WINMGR_DIR}/WinMAnager.cpp)

set(UV_DIR ${SRC_DIR}/UV)
set(UV_SRC 
    ${UV_DIR}/TexturePresampler.cpp
    ${UV_DIR}/MatPrevRender.cpp)

set(RSRC_DIR ${SRC_DIR}/resource)

set(FONT_DIR ${RSRC_DIR}/fonts)
set(FONT_SRC ${FONT_DIR}/Rubik-Regular.hpp)


set(IMGUI_DIR ./externals/IMGuiDocking)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
set(IMGUI_BACK_DIR
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp 
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
)

set(GLTF_DIR ./externals/gltf)
include_directories(${GLTF_DIR} ${GLTF_DIR}/fx ${IMGUI_DIR}/meshoptimizer ${IMGUI_DIR}/nlohmann ..)
set(GLTF_HEAD_DIR
    ${GLTF_DIR}/fx/gltf.h 
    ${GLTF_DIR}/meshoptimizer/meshoptimizer.h
    ${GLTF_DIR}/nlohmann/json.hpp

    ${GLTF_DIR}/ImageData.h
    ${GLTF_DIR}/MaterialData.h
    ${GLTF_DIR}/MeshData.h
)

set(MESHOPT_DIR ./externals/gltf/meshoptimizer)
set(GLTF_HEAD_DIR
    ${MESHOPT_DIR}/allocator.cpp
    ${MESHOPT_DIR}/clusterizer.cpp
    ${MESHOPT_DIR}/indexcodec.cpp
    ${MESHOPT_DIR}/indexgenerator.cpp
    ${MESHOPT_DIR}/overdrawanalyzer.cpp
    ${MESHOPT_DIR}/simplifier.cpp
    ${MESHOPT_DIR}/spatialorder.cpp
    ${MESHOPT_DIR}/stripifier.cpp
    ${MESHOPT_DIR}/vcacheanalyzer.cpp
    ${MESHOPT_DIR}/vcacheoptimizer.cpp
    ${MESHOPT_DIR}/vertexcodec.cpp
    ${MESHOPT_DIR}/vertexfilter.cpp
    ${MESHOPT_DIR}/vfetchanalyzer.cpp
    ${MESHOPT_DIR}/vfetchoptimizer.cpp
)

set(GLV_DIR ./general)

set(GLVGLTF_DIR ./GLVgltf)
set(GLVGLTFHEAD_DIR
    ${GLVGLTF_DIR}/GLVElement.cpp
    ${GLVGLTF_DIR}/GLVMesh.cpp
    ${GLVGLTF_DIR}/GLVCamera.cpp
    ${GLVGLTF_DIR}/GLVMaterial.cpp
    ${GLVGLTF_DIR}/GLVMeshes.cpp
    ${GLVGLTF_DIR}/GLVNode.cpp
    ${GLVGLTF_DIR}/GLVPrimitive.cpp
    ${GLVGLTF_DIR}/GLVScene.cpp
    ${GLVGLTF_DIR}/GLVTexture.cpp
)

set(GLVWIN32_DIR ./GLVWin32)
set(GLVWIN32HEAD_DIR 
    ${GLVWIN32_DIR}/GLVWindow.cpp
)

include_directories(
    externals/glm-0.9.9.8
    externals/imgui
    externals/gltf
    externals/stb_image
    .
)

add_executable(GLV
    Sample_main.cpp
    general/loadshader.hpp
    general/loadshader.cpp
    general/objloader.hpp
    general/objloader.cpp
    general/mtlloader.hpp
    general/mtlloader.cpp
    general/texture.hpp
    general/texture.cpp

    externals/stb_image/stb_image.h
    general/GLVBuffer/IndexBuffer.cpp
    general/GLVBuffer/VertexBuffer.cpp
    general/GLVVertexArray/VertexArray.cpp
    general/GLVVertexArray/BufferLayout.cpp
    general/GLVBuffer/UniformBuffer.cpp
    general/GLVBuffer/AbsBuffer.cpp
    general/GLVBuffer/FrameBuffer.cpp

    general/GLVFileLoad/FileManager.cpp
    general/GLVFileLoad/GLVFile.cpp
    source/Manager/sysRsrcManager.cpp


    GLVUI/SceneView.cpp

    source/incls.h

        ${IMGUI_BACK_DIR}
        ${GLTF_HEAD_DIR}
        ${GLVGLTFHEAD_DIR}
        ${GLVWIN32HEAD_DIR}
        ${CTX_SRC}
        ${UICOMP_SRC}
        ${WINMGR_SRC}
        ${SCENE_SRC}
        ${UV_SRC}
        ${FONT_SRC}
    )

set_target_properties(GLV PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(GLV PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

target_link_libraries(GLV -m64 -lOpenGL32 -lglew32 -ldwmapi)
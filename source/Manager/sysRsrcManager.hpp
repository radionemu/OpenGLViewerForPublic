#pragma once
#include <string>

namespace Resources
{

    class sysRsrcManager
    {
    private:
        unsigned int ico_file_black;
        unsigned int ico_file_white;

        unsigned int ico_scene_black;
        unsigned int ico_scene_color;
        unsigned int ico_scene_white;

        unsigned int ico_camera_black;
        unsigned int ico_camera_color;
        unsigned int ico_camera_white;

        unsigned int ico_image_black;
        unsigned int ico_image_color;
        unsigned int ico_image_white;

        unsigned int ico_material_black;
        unsigned int ico_material_color;
        unsigned int ico_material_white;

        unsigned int ico_mesh_black;
        unsigned int ico_mesh_color;
        unsigned int ico_mesh_white;

        unsigned int ico_nodeNormal_black;
        unsigned int ico_nodeNormal_color;
        unsigned int ico_nodeNormal_white;

        unsigned int ico_nodeMesh_black;
        unsigned int ico_nodeMesh_color;
        unsigned int ico_nodeMesh_white;

        unsigned int ico_primitive_black;
        unsigned int ico_primitive_color;
        unsigned int ico_primitive_white;

        std::string rootDir = "./imgsource/";
    public:
        sysRsrcManager(/* args */);
        ~sysRsrcManager();
        void init();
        unsigned int getIconID(unsigned int iconenum);
    };
} // namespace GLVGLTF

    enum icons{
        icons_File_Black,
        icons_File_White,
        icons_Scene_Black,
        icons_Scene_Color,
        icons_Scene_White,
        icons_Camera_Black,
        icons_Camera_Color,
        icons_Camera_White,
        icons_Image_Black,
        icons_Image_Color,
        icons_Image_White,
        icons_Material_Black,
        icons_Material_Color,
        icons_Material_White,
        icons_Mesh_Black,
        icons_Mesh_Color,
        icons_Mesh_White,
        icons_NodeNormal_Black,
        icons_NodeNormal_Color,
        icons_NodeNormal_White,
        icons_NodeMesh_Black,
        icons_NodeMesh_Color,
        icons_NodeMesh_White,
        icons_Primitive_Black,
        icons_Primitive_Color,
        icons_Primitive_White
    };
#include "sysRsrcManager.hpp"
#include "general/texture.hpp"

namespace Resources
{

    sysRsrcManager::sysRsrcManager()
    {
    }

    sysRsrcManager::~sysRsrcManager()
    {
    }

    void sysRsrcManager::init()
    {
        this->ico_file_white = LoadPNG((rootDir+"file_white.png").c_str());
        
        this->ico_scene_black = LoadPNG((rootDir+"scene.png").c_str());
        this->ico_scene_color = LoadPNG((rootDir+"scene_color.png").c_str());
        this->ico_scene_white = LoadPNG((rootDir+"scene_white.png").c_str());

        this->ico_camera_black = LoadPNG((rootDir+"camera.png").c_str());
        this->ico_camera_color = LoadPNG((rootDir+"camera_color.png").c_str());
        this->ico_camera_white = LoadPNG((rootDir+"camera_white.png").c_str());

        this->ico_image_black = LoadPNG((rootDir+"image.png").c_str());
        this->ico_image_color = LoadPNG((rootDir+"image_color.png").c_str());
        this->ico_image_white = LoadPNG((rootDir+"image_white.png").c_str());

        this->ico_material_black = LoadPNG((rootDir+"material.png").c_str());
        this->ico_material_color = LoadPNG((rootDir+"material_color.png").c_str());
        this->ico_material_white = LoadPNG((rootDir+"material_white.png").c_str());

        this->ico_mesh_black = LoadPNG((rootDir+"mesh.png").c_str());
        this->ico_mesh_color = LoadPNG((rootDir+"mesh_color.png").c_str());
        this->ico_mesh_white = LoadPNG((rootDir+"mesh_white.png").c_str());

        this->ico_nodeNormal_black = LoadPNG((rootDir+"normalNode.png").c_str());
        this->ico_nodeNormal_color = LoadPNG((rootDir+"normalNode_color.png").c_str());
        this->ico_nodeNormal_white = LoadPNG((rootDir+"normalNode_white.png").c_str());

        this->ico_nodeMesh_black = LoadPNG((rootDir+"meshNode.png").c_str());
        this->ico_nodeMesh_color = LoadPNG((rootDir+"meshNode_color.png").c_str());
        this->ico_nodeMesh_white = LoadPNG((rootDir+"meshNode_white.png").c_str());

        this->ico_primitive_black = LoadPNG((rootDir+"primitive.png").c_str());
        this->ico_primitive_color = LoadPNG((rootDir+"primitive_color.png").c_str());
        this->ico_primitive_white = LoadPNG((rootDir+"primitive_white.png").c_str());
    }

    unsigned int sysRsrcManager::getIconID(unsigned int iconenum)
    {
        if(iconenum == icons_File_Black) return this->ico_file_black;
        if(iconenum == icons_File_White) return this->ico_file_white;
        if(iconenum == icons_Scene_Black) return this->ico_scene_black;
        if(iconenum == icons_Scene_Color) return this->ico_scene_color;
        if(iconenum == icons_Scene_White) return this->ico_scene_white;
        if(iconenum == icons_Camera_Black) return this->ico_camera_black;
        if(iconenum == icons_Camera_Color) return this->ico_camera_color;
        if(iconenum == icons_Camera_White) return this->ico_camera_white;
        if(iconenum == icons_Image_Black) return this->ico_image_black;
        if(iconenum == icons_Image_Color) return this->ico_image_color;
        if(iconenum == icons_Image_White) return this->ico_image_white;
        if(iconenum == icons_Material_Black) return this->ico_material_black;
        if(iconenum == icons_Material_Color) return this->ico_material_color;
        if(iconenum == icons_Material_White) return this->ico_material_white;
        if(iconenum == icons_Mesh_Black) return this->ico_mesh_black;
        if(iconenum == icons_Mesh_Color) return this->ico_mesh_color;
        if(iconenum == icons_Mesh_White) return this->ico_mesh_white;
        if(iconenum == icons_NodeNormal_Black) return this->ico_nodeNormal_black;
        if(iconenum == icons_NodeNormal_Color) return this->ico_nodeNormal_color;
        if(iconenum == icons_NodeNormal_White) return this->ico_nodeNormal_white;
        if(iconenum == icons_NodeMesh_Black) return this->ico_nodeMesh_black;
        if(iconenum == icons_NodeMesh_Color) return this->ico_nodeMesh_color;
        if(iconenum == icons_NodeMesh_White) return this->ico_nodeMesh_white; 
        if(iconenum == icons_Primitive_Black) return this->ico_primitive_black;
        if(iconenum == icons_Primitive_Color) return this->ico_primitive_color;
        if(iconenum == icons_Primitive_White) return this->ico_primitive_white;
        return 0;
    }

    
} // namespace Resources



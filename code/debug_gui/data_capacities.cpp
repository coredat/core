#include <debug_gui/data_capacities.hpp>
#include <data/world/camera_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/rigidbody_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/world/transform_data.hpp>
#include <data/context/text_mesh_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/mesh_data.hpp>
#include <data/context/post_process_data.hpp>
#include <data/context/shader_data.hpp>
#include <data/context/texture_data.hpp>

#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_capacities(Data::Text_mesh_data *text_mesh_data,
                   Data::Material_data *material_data,
                   Data::Mesh_data *mesh_data,
                   Data::Post_process_data *post_process_data,
                   Data::Shader_data *shader_data,
                   Data::Texture_data *texture_data,
                   
                   Data::Camera_data *camera_data,
                   Data::Entity_data *entity_data,
                   Data::Mesh_draw_call_data *renderer_mesh_data,
                   Data::Text_draw_call_data *renderer_text_draw_calls,
                   Data::Transform_data *transforms,
                   Data::Rigidbody_data *phs_data)
{
  ImGui::Begin("Data Capacities");
  {
    auto display_capacity = [](const char *name, const auto data)
    {
      char buf[32];
    
      ImGui::TextWrapped("%s", name);
      {
        const float progress = static_cast<float>(data->size) / static_cast<float>(data->capacity);
        sprintf(buf, "%zu/%zu", data->size, data->capacity);
        ImGui::ProgressBar(progress, ImVec2(-1.f,0.f), buf);
      }
    };
    
    ImGui::TextWrapped("World data\n");
    
    display_capacity("Entities", entity_data);
    display_capacity("Transforms", transforms);
    display_capacity("Material Draw Calls", renderer_mesh_data);
    display_capacity("Text Draw Calls", renderer_text_draw_calls);
    display_capacity("Cameras", camera_data);
    
    ImGui::Separator();
    
    ImGui::TextWrapped("Global data\n");
    
    display_capacity("Materials", material_data);
    display_capacity("Meshes", mesh_data);
    display_capacity("Text Meshes", text_mesh_data);
    display_capacity("Post", post_process_data);
    display_capacity("Shaders", shader_data);
    display_capacity("Texture", texture_data);
    display_capacity("Physics", phs_data);
  }
  ImGui::End();
}


} // ns
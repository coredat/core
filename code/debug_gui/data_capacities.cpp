#include <debug_gui/data_capacities.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_capacities(Resource_data::Text_mesh_data *text_mesh_data,
                   Resource_data::Material_data *material_data,
                   Resource_data::Mesh_data *mesh_data,
                   Resource_data::Post_process_data *post_process_data,
                   Resource_data::Shader_data *shader_data,
                   Resource_data::Texture_data *texture_data,
                   
                   World_data::Camera_data *camera_data,
                   World_data::Entity_data *entity_data,
                   World_data::Renderer_mesh_data *renderer_mesh_data,
                   World_data::Renderer_text_draw_calls_data *renderer_text_draw_calls,
                   World_data::Transform_data *transforms)
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
  }
  ImGui::End();
}


} // ns
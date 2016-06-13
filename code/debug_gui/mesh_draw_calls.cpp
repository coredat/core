#include <debug_gui/mesh_draw_calls.hpp>
#include <3rdparty/imgui/imgui.h>
#include <stdio.h>


namespace Debug_menu {


void
display_mesh_draw_calls(World_data::Renderer_mesh_data *data)
{
  ImGui::Begin("Mesh Renderer Draw Calls");
  {
    ImGui::Columns(2, "mesh_draw_calls");
    ImGui::Separator();
    
    ImGui::Text("Entity ID"); ImGui::NextColumn();
    ImGui::Text("Material ID"); ImGui::NextColumn();
    
    ImGui::Separator();
        
    for(uint32_t i = 0; i < data->size; ++i)
    {
      ImGui::Text("%u", data->entity_id[i]); ImGui::NextColumn();
      ImGui::Text("%u", data->property_material_id[i]); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns
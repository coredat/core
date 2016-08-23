#include <debug_gui/material_list.hpp>
#include <systems/renderer_material/material.hpp>
#include <data/global_data/material_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <stdio.h>


namespace Debug_menu {


void
display_material_list(Resource_data::Material_data *data)
{
  ImGui::Begin("Material List");
  {
    ImGui::Columns(5, "material_cols");
    ImGui::Separator();
    
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Priority Key"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Shader"); ImGui::NextColumn();
    ImGui::Text("Map 01"); ImGui::NextColumn();
    
    ImGui::Separator();
    
    static int selected = -1;
    
    for(uint32_t i = 0; i < data->size; ++i)
    {
      char id[4];
      sprintf(id, "%02d", data->material_id[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      
      ImGui::NextColumn();
      
      ImGui::Text("%d", *reinterpret_cast<uint32_t*>(&data->property_material_hash_id[i]));

      ImGui::NextColumn();
      
      const char *name;
      Resource_data::material_data_get_property_name(data, data->material_id[i], &name);
      ImGui::Text("%s", name);
      
      ImGui::NextColumn();
      
      ::Material_renderer::Material *material;
      Resource_data::material_data_get_property_material(data, data->material_id[i], &material);
      
      ImGui::Text("%02d", material->shader.program_id); ImGui::NextColumn();
      ImGui::Text("%02d", material->map_01.texture_id); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns
#include <debug_gui/material_list.hpp>
#include <systems/renderer_material/material.hpp>
#include <data/context/material_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <stdio.h>


namespace Debug_menu {


void
display_material_list(Data::Material_data *data)
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
      sprintf(id, "%02d", data->keys[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      
      ImGui::NextColumn();
      
      ImGui::Text("%d", *reinterpret_cast<uint32_t*>(&data->field_material_hash[i]));

      ImGui::NextColumn();
      
      const char *name;
      Data::material_get_name(data, data->keys[i], &name);
      ImGui::Text("%s", name);
      
      ImGui::NextColumn();
      
      ::Material_renderer::Material material;
      Data::material_get_material(data, data->keys[i], &material);
      
      ImGui::Text("%02d", material.shader.program_id); ImGui::NextColumn();
      ImGui::Text("%02d", material.map_01.texture_id); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns
#include <debug_gui/shader_list.hpp>
#include <data/context/shader_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <stdio.h>


namespace Debug_menu {


void
display_shader_list(Data::Shader_data *data)
{
  ImGui::Begin("Shader List");
  {
    ImGui::Columns(2, "shader_cols");
    ImGui::Separator();
    
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    
    ImGui::Separator();
    
    static int selected = -1;
    
    for(uint32_t i = 0; i < data->size; ++i)
    {
      char id[4];
      sprintf(id, "%02d", data->keys[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      ImGui::NextColumn();
      const char *name;
      
      Data::shader_get_name(data, data->keys[i], &name);
      ImGui::Text("%s", name); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns
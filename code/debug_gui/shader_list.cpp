#include <debug_gui/texture_list.hpp>
#include <data/global_data/shader_data.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_shader_list(Resource_data::Shader_data *data)
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
      sprintf(id, "%02d", data->shader_ids[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      ImGui::NextColumn();
      ImGui::Text(Resource_data::shader_data_get_name(data, i)); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
    
  }
  ImGui::End();
}


} // ns
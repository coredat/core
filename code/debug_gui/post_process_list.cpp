#include <debug_gui/post_process_list.hpp>
#include <data/global_data/post_process_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <stdio.h>


namespace Debug_menu {


void
display_post_process_list(Resource_data::Post_process_data *data)
{
  ImGui::Begin("Post Process List");
  {
    ImGui::Columns(2, "post list");
    ImGui::Separator();
    
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    
    ImGui::Separator();
    
    static int selected = -1;
    
    for(uint32_t i = 0; i < data->size; ++i)
    {
      char id[4];
      sprintf(id, "%02d", data->post_process_id[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      ImGui::NextColumn();
      const char *name;
      
      Resource_data::post_process_data_get_property_name(data, data->post_process_id[i], &name);
      ImGui::Text("%s", name); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns
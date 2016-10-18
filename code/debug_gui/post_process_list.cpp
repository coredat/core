#include <debug_gui/post_process_list.hpp>
#include <data/context/post_process_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <stdio.h>


namespace Debug_menu {


void
display_post_process_list(Data::Post_process_data *data)
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
      sprintf(id, "%02d", data->keys[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      ImGui::NextColumn();
      const char *name;
      
      Data::post_process_get_name(data, data->keys[i], &name);
      ImGui::Text("%s", name); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns
#include <debug_gui/text_mesh.hpp>
#include <data/context/text_mesh_data.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_text_mesh_list(Data::Text_mesh_data *data)
{
  ImGui::Begin("Text Mesh Data");
  {
    for(uint32_t i = 0; i < data->size; ++i)
    {
      const char *text;
      Data::text_mesh_get_text(data, data->keys[i], &text);
      
      ImGui::Text("ID: %d", data->keys[i]);
      ImGui::Text("Text: %s", text);
      
      ImGui::Separator();
    }
  }
  ImGui::End();
}


} // ns
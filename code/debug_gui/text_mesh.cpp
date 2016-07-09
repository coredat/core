#include <debug_gui/text_mesh.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_text_mesh_list(Resource_data::Text_mesh_data *data)
{
  ImGui::Begin("Text Mesh Data");
  {
    for(uint32_t i = 0; i < data->size; ++i)
    {
      const char *text;
      Resource_data::text_mesh_data_get_property_text(data, data->text_mesh_id[i], &text);
      
      ImGui::Text("ID: %d", data->text_mesh_id[i]);
      ImGui::Text("Text: %s", text);
      
      ImGui::Separator();
    }
  }
  ImGui::End();
}


} // ns
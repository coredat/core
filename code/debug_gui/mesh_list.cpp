#include <debug_gui/shader_list.hpp>
#include <data/context/mesh_data.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_mesh_list(Data::Mesh_data *data)
{
  ImGui::Begin("Mesh List");
  {
    ImGui::Columns(3, "mesh_cols");
    ImGui::Separator();
    
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Number of Entries"); ImGui::NextColumn();
    
    ImGui::Separator();
    
    static int selected = -1;
    
    for(uint32_t i = 0; i < data->size; ++i)
    {
      char id[4];
      sprintf(id, "%02d", data->keys[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      ImGui::NextColumn();
      const char *name;
      
      Data::mesh_get_name(data, data->keys[i], &name);
      ImGui::Text("%s", name); ImGui::NextColumn();
      
      Graphics_api::Mesh mesh;
      Data::mesh_get_mesh(data, data->keys[i], &mesh);
      ImGui::Text("%d", mesh.vbo.number_of_entries); ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns
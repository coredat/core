#include <debug_gui/camera_list.hpp>
#include <data/world/camera_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <stdio.h>


namespace Debug_menu {


void
display_camera_data(Data::Camera_data *data)
{
  ImGui::Begin("Camera List");
  {
    ImGui::Columns(11, "material_cols");
    ImGui::Separator();
    
    ImGui::Text("ID");          ImGui::NextColumn();
    ImGui::Text("Entity ID");   ImGui::NextColumn();
    ImGui::Text("Post ID");     ImGui::NextColumn();
    ImGui::Text("Texture ID");  ImGui::NextColumn();
    ImGui::Text("Priority");    ImGui::NextColumn();
    ImGui::Text("Clear Flags"); ImGui::NextColumn();
    ImGui::Text("Type");        ImGui::NextColumn();
    ImGui::Text("FOV");         ImGui::NextColumn();
    ImGui::Text("Mask");        ImGui::NextColumn();
    ImGui::Text("Viewport");    ImGui::NextColumn();
    ImGui::Text("Planes");      ImGui::NextColumn();
    
    ImGui::Separator();
    
    static int selected = -1;
    
    for(uint32_t i = 0; i < data->size; ++i)
    {
      char id[4];
      sprintf(id, "%02d", data->keys[i]);
    
      ImGui::Selectable(id, selected == i, ImGuiSelectableFlags_SpanAllColumns);
      ImGui::NextColumn();
      
//      ImGui::Text("%04d", data->field_entity_id[i]);       ImGui::NextColumn();
      ImGui::Text("%02d", data->field_post_process_id[i]); ImGui::NextColumn();
      ImGui::Text("%02d", data->field_texture_id[i]);      ImGui::NextColumn();
      
      ImGui::Text("%d", *reinterpret_cast<uint32_t*>(&data->field_priority[i]));
      ImGui::NextColumn();
      
      ::Camera::Camera_properties props;
      Data::camera_get_properties(data, data->keys[i], &props);

      ImGui::Text("%d",           props.clear_flags);                                                  ImGui::NextColumn();
      ImGui::Text("%s",           (props.type == Core::Camera_type::orthographic ? "Orth" : "Perp"));  ImGui::NextColumn();
      ImGui::Text("%.2f",         props.fov);                                                          ImGui::NextColumn();
      ImGui::Text("%d",           props.cull_mask);                                                    ImGui::NextColumn();
      ImGui::Text("%d x %d",      props.viewport_width, props.viewport_height);                        ImGui::NextColumn();
      ImGui::Text("%.1f - %.1f",  props.near_plane, props.far_plane);                                  ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::Separator();
  }
  ImGui::End();

}


} // ns
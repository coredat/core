#include <debug_gui/texture_list.hpp>
#include <data/global_data/texture_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <graphics_api/ogl/ogl_pixel_format.hpp>


namespace Debug_menu {


void
display_texture_list(Resource_data::Texture_data *data)
{
  ImGui::Begin("Texture List");
  
  uint32_t count = 0;
  
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    if(data->texture_id[i] != 0)
    {
      if(count > 3)
      {
        count = 0;
      }
    
      if(count > 0)
      {
        ImGui::SameLine();
      }
      
      count++;
      
      Ogl::Texture tex = data->property_texture[i];

      ImGui::PushID(count);
      ImGui::Image((void*)(uintptr_t)tex.texture_id, // casting :/
                   ImVec2(128, 128),
                   ImVec2(0,0),
                   ImVec2(1,1),
                   ImColor(255,255,255,255),
                   ImColor(255,255,255,128));
      
      const char *name;
      Resource_data::texture_data_get_property_name(data, data->texture_id[i], &name);
      if (ImGui::IsItemHovered())
      {
        ImGui::SetTooltip("Name: %s, Width: %d, Height: %d, Format: %s", name, tex.width, tex.height, Ogl::pixel_format_get_name(tex.format));
      }


      ImGui::PopID();
    }
  }
  
  ImGui::End();
}


} // ns
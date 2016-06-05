#include <debug_gui/texture_list.hpp>
#include <data/global_data/texture_pool.hpp>
#include <3rdparty/imgui/imgui.h>


namespace Debug_menu {


void
display_texture_list(Resource_data::Texture_pool *data)
{
  ImGui::Begin("Texture List");
  
  uint32_t count = 0;
  
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    if(data->id[i] != 0)
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
      
      ImGui::PushID(count);
      ImGui::Image((void*)data->texture[i].texture_id,
                   ImVec2(64, 64),
                   ImVec2(0,0),
                   ImVec2(1,1),
                   ImColor(255,255,255,255),
                   ImColor(255,255,255,128));

      ImGui::PopID();
    }
  }
  
  ImGui::End();
}


} // ns
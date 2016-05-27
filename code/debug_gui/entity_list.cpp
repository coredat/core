#include <debug_gui/entity_list.hpp>
#include <data/world_data/entity_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <string>


namespace Debug_menu {


void
display_entity_list(World_data::Entity_data *data)
{
  ImGui::Begin("Entities");
  {
    ImGui::TextWrapped("Currently %d entities in memory", data->size);
    ImGui::Separator();
  
    for(uint32_t i = 0; i < data->size; ++i)
    {
      ImGui::Text("%d", data->entity_id[i]);
      ImGui::Text("%s", World_data::entity_data_get_name(data, data->entity_id[i]));
      ImGui::Text("%d", data->tags[i]);
      ImGui::Text("%p", (void*)data->user_data[i]);
      
      ImGui::Separator();
    }
  }
  ImGui::End();
}


} // ns
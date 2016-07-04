#include <debug_gui/entity_list.hpp>
#include <data/world_data/entity_data.hpp>
#include <3rdparty/imgui/imgui.h>


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
      
      const char *name;
      World_data::entity_data_get_property_name(data, data->entity_id[i], &name);
      
      ImGui::Text("%s", name);
      ImGui::Text("%d", data->property_tag[i]);
      ImGui::Text("%p", (void*)data->property_user_data[i]);
      
      ImGui::Separator();
    }
  }
  ImGui::End();
}


} // ns
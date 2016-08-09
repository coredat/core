

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Tue 09 Aug 2016
*/


#include <3rdparty/imgui/imgui.h>
#include "debug_ui_entity_data.hpp"


namespace Debug_menu {


void
display_entity_data(const World_data::Entity_data *data)
{
  ImGui::Begin("Entity data");
  {
    // Container Summary
    ImGui::TextWrapped("Entity data container");
    ImGui::TextWrapped("size/capacity: %zu/%zu", data->size, data->capacity);
    ImGui::Separator();

    // List contents
    for(size_t i = 0; i < data->size; ++i)
    {
      // Key
      ImGui::Text("Key: %d", data->entity_id[i]);

      // Get property_name
      const char *property_name_str;
      World_data::entity_data_get_property_name(data, data->entity_id[i], &property_name_str);
      ImGui::Text("Name %s", property_name_str);
      // Get property tag
      uint32_t property_tag_value;
      World_data::entity_data_get_property_tag(data, data->entity_id[i], &property_tag_value);
      ImGui::Text("Tag: %d", property_tag_value);
      // Get property components
      uint32_t property_components_value;
      World_data::entity_data_get_property_components(data, data->entity_id[i], &property_components_value);
      ImGui::Text("Components: %d", property_components_value);
      // Get property_user_data
      uintptr_t property_user_data_value;
      World_data::entity_data_get_property_user_data(data, data->entity_id[i], &property_user_data_value);
      ImGui::Text("User data: %p", (void*)property_user_data_value);
      // Get property renderer
      uint32_t property_renderer_value;
      World_data::entity_data_get_property_renderer(data, data->entity_id[i], &property_renderer_value);
      ImGui::Text("Renderer: %d", property_renderer_value);

      // End of data
      ImGui::Separator();
    }

  }
  ImGui::End();
}


} // ns

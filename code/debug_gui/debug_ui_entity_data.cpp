

/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: Sun 09 Oct 2016
*/


#include <3rdparty/imgui/imgui.h>
//#include <data/world/entity_data.hpp>
#include "debug_ui_entity_data.hpp"


namespace Debug_menu {


void
display_entity_data(const Data::Entity_data *data)
{
//  ImGui::Begin("Entity data");
//  {
//    // Container Summary
//    ImGui::TextWrapped("Entity data container");
//    ImGui::TextWrapped("size/capacity: %zu/%zu", data->size, data->capacity);
//    ImGui::Separator();
//
//    // List contents
//    for(size_t i = 0; i < data->size; ++i)
//    {
//      // Key
//      ImGui::Text("Key: %d", data->keys[i]);
//
//
//      // Get property_name
//      const char *property_name_str;
//      Data::entity_get_name(data, data->keys[i], &property_name_str);
//      ImGui::Text("Name %s", property_name_str);
//
//
//      // Get property tag
//      uint32_t property_tag_value;
//      Data::entity_get_tags(data, data->keys[i], &property_tag_value);
//      ImGui::Text("Tag: %d", property_tag_value);
//
//
//      // Get property components
//      uint32_t property_components_value;
//      Data::entity_get_components(data, data->keys[i], &property_components_value);
//      ImGui::Text("Components: %d", property_components_value);
//
//
//      // Get property_user_data
//      uintptr_t property_user_data_value;
//      Data::entity_get_user_data(data, data->keys[i], &property_user_data_value);
//      ImGui::Text("User data: %p", (void*)property_user_data_value);
//
//      // End of data
//      ImGui::Separator();
//    }
//
//  }
//  ImGui::End();
}


} // ns

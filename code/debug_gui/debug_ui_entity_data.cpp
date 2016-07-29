

/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/


#include <3rdparty/imgui/imgui.h>
#include "debug_ui_entity_data.hpp"


namespace Debug_menu {


void
display_entity_data(const World_data::Entity_data *data)
{
  ImGui::Begin("entity_data");
  {
    // Container Summary
    ImGui::TextWrapped("entity_data container");
    ImGui::TextWrapped("size/capacity: %zu/%zu", data->size, data->capacity);
    ImGui::Separator();
  }
  ImGui::End();
}


} // ns

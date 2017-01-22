#include <debug_gui/entity_list.hpp>
//#include <data/world/entity_data.hpp>
#include <3rdparty/imgui/imgui.h>
#include <core/renderer/renderer.hpp>


namespace Debug_menu {


void
display_entity_list(Data::Entity_data *data)
{
//  ImGui::Begin("Entities");
//  {
//    ImGui::TextWrapped("Currently %zu entities in memory", data->size);
//    ImGui::Separator();
//  
//    for(uint32_t i = 0; i < data->size; ++i)
//    {
//      ImGui::Text("%d", data->keys[i]);
//      
//      const char *name;
//      Data::entity_get_name(data, data->keys[i], &name);
//      
//      ImGui::Text("%s", name);
//      ImGui::Text("%d", data->field_tags[i]);
//      ImGui::Text("%p", (void*)data->field_user_data[i]);
//      
//      ImGui::Separator();
//    }
//  }
//  ImGui::End();
}


} // ns
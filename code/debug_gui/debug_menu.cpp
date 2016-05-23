#include <debug_gui/debug_menu.hpp>
#include <debug_gui/texture_list.hpp>
#include <debug_gui/shader_list.hpp>
#include <debug_gui/entity_list.hpp>

#include <3rdparty/imgui/imgui.h>

#include <data/global_data/resource_data.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/entity_data.hpp>


namespace Debug_menu {

namespace
{
  bool show_texture_list = false;
  bool show_shader_list = false;
}

void
display_global_data_menu()
{
  auto global_data = Resource_data::get_resources();

  if(!global_data)
  {
    return;
  }

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("Global Data"))
    {
      ImGui::MenuItem("Texture", nullptr, &show_texture_list);
      ImGui::MenuItem("Shaders", nullptr, &show_shader_list);
      
      ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
  }
  
  if(show_texture_list) { display_texture_list(global_data->texture_pool);  }
  if(show_shader_list)  { display_shader_list(global_data->shader_data);    }
}


namespace
{
  bool show_entity_list = false;
}


void
dispaly_world_data_menu(World_data::World *world_data)
{
  if(!world_data)
  {
    return;
  }

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("World Data"))
    {
      ImGui::MenuItem("Entities", nullptr, &show_entity_list);
      
      ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
  }
  
  if(show_entity_list) { display_entity_list(world_data->entity); }
}


} // ns
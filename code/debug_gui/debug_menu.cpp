#include <debug_gui/debug_menu.hpp>
#include <debug_gui/texture_list.hpp>
#include <debug_gui/shader_list.hpp>
#include <debug_gui/entity_list.hpp>
#include <debug_gui/memory_view.hpp>
#include <debug_gui/mesh_list.hpp>
#include <debug_gui/material_list.hpp>
#include <debug_gui/mesh_draw_calls.hpp>
#include <debug_gui/post_process_list.hpp>
#include <debug_gui/camera_list.hpp>
#include <debug_gui/stats_overlay.hpp>
#include <debug_gui/text_mesh.hpp>
#include <debug_gui/data_capacities.hpp>
#include <debug_gui/input_data.hpp>

#include <data/world_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <data/world_data.hpp>
#include <data/context_data/input_pool.hpp>

#include <3rdparty/imgui/imgui.h>

// Auto generated
#include "debug_ui_entity_data.hpp"
#include "debug_ui_transform_data.hpp"


namespace Debug_menu {

#ifdef CORE_DEBUG_MENU
namespace
{
  bool show_texture_list   = false;
  bool show_model_list     = false;
  bool show_shader_list    = false;
  bool show_memory_view    = false;
  bool show_material_list  = false;
  bool show_post_list      = false;
  bool show_text_mesh_list = false;
  bool show_input_data     = false;
}
#endif

void
display_global_data_menu(const Context_data::Input_pool *input_data)
{
  #ifdef CORE_DEBUG_MENU
  auto global_data = Resource_data::get_resource_data();

  if(!global_data)
  {
    return;
  }

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("Global Data"))
    {
      ImGui::MenuItem("Texture",    nullptr, &show_texture_list);
      ImGui::MenuItem("Model",      nullptr, &show_model_list);
      ImGui::MenuItem("Shaders",    nullptr, &show_shader_list);
      ImGui::MenuItem("Memory",     nullptr, &show_memory_view);
      ImGui::MenuItem("Material",   nullptr, &show_material_list);
      ImGui::MenuItem("Post",       nullptr, &show_post_list);
      ImGui::MenuItem("Text Mesh",  nullptr, &show_text_mesh_list);
      ImGui::MenuItem("Input Data", nullptr, &show_input_data);
      
      ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
  }
  
  if(show_texture_list)   { display_texture_list(global_data->texture_data);        }
  if(show_model_list)     { display_mesh_list(global_data->mesh_data);              }
  if(show_shader_list)    { display_shader_list(global_data->shader_data);          }
  if(show_memory_view)    { display_memory_useage(Memory::_get_pool());             }
  if(show_material_list)  { display_material_list(global_data->material_data);      }
  if(show_post_list)      { display_post_process_list(global_data->post_data);      }
  if(show_text_mesh_list) { display_text_mesh_list((global_data->text_mesh_data));  }
  if(show_input_data)     { display_input_data(input_data);                         }
  #endif
}


#ifdef CORE_DEBUG_MENU
namespace
{
  bool show_entity_list     = false;
  bool show_transforms      = false;
  bool show_mesh_draw_calls = false;
  bool show_camera_list     = false;
  bool show_world_stats     = false;
  bool show_capacities      = false;
}
#endif


void
display_world_data_menu(Data::World *world_data,
                        const float dt,
                        const float dt_mul,
                        const uint32_t number_of_rbs,
                        const uint32_t draw_calls,
                        const uint32_t render_passes)
{
  #ifdef CORE_DEBUG_MENU
  if(!world_data)
  {
    return;
  }

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("World Data"))
    {
      ImGui::MenuItem("Entities",         nullptr, &show_entity_list);
      ImGui::MenuItem("Transforms",       nullptr, &show_transforms);
      ImGui::MenuItem("Mesh Draw Calls",  nullptr, &show_mesh_draw_calls);
      ImGui::MenuItem("Cameras",          nullptr, &show_camera_list);
      ImGui::MenuItem("Stats",            nullptr, &show_world_stats);
      ImGui::MenuItem("Capacities",       nullptr, &show_capacities);
      ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
  }
  
  if(show_entity_list)     { display_entity_data(world_data->entity);                                   }
  if(show_transforms)      { display_transform_data(world_data->transform);                             }
  if(show_mesh_draw_calls) { display_mesh_draw_calls(world_data->mesh_data);                            }
  if(show_camera_list)     { display_camera_data(world_data->camera_data);                              }
  if(show_world_stats)     { display_world_stats(dt, dt_mul, draw_calls, render_passes, number_of_rbs); }
  
  auto global_data = Resource_data::get_resource_data();

  if(!global_data)
  {
    return;
  }
  
  if(show_capacities)
  {
    display_capacities(global_data->text_mesh_data,
                       global_data->material_data,
                       global_data->mesh_data,
                       global_data->post_data,
                       global_data->shader_data,
                       global_data->texture_data,
                       
                       world_data->camera_data,
                       world_data->entity,
                       world_data->mesh_data,
                       world_data->text_data,
                       world_data->transform,
                       world_data->rigidbody_data);
  }
  #endif
}


} // ns
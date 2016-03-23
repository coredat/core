#include <application/resources.hpp>
#include <application/entity_factory.hpp>
#include <application/common/ids_object_tags.hpp>

#include <systems/transform/transformations.hpp>
#include <systems/network/network.hpp>
#include <systems/network/network_connection.hpp>
#include <systems/entity/generic_id.hpp>
#include <systems/environment/environment.hpp>

#include <core/gui/gui_view.hpp>

#include <math/math.hpp>
#include <utilities/timer.hpp>

#include <iostream>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>

#include <application/engine/application_client.hpp>
#include <application/engine/application_host.hpp>
#include <application/engine/application_common.hpp>
#include <application/engine/application_graphics.hpp>

#include <core/world/world.hpp>
#include <vector>

#include <graphics_api/clear.hpp>

#ifdef main
#undef main
#endif

#ifndef UNIT_TEST
int
main(int argc, char *argv[])
{
  Network::Connection *connection = nullptr;

  const std::string title = "Application";
  Environment::Window window;
  Environment::window_create(&window, 800, 480, false, title.c_str());

  Core::World world(Core::World_setup{});

  Resource::load_default_resources(&world.m_world_data);
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  
  util::timer frame_timer;
  frame_timer.start();
  
  Graphics_api::clear_color_set(0.4f, 0.2f, 0.2f);

  // Foop
  bool run_game = true;
  
  while(run_game)
  {
    const float delta_time = static_cast<float>(frame_timer.split()) / 1000.f;

    // ** Common ** //
    
    Application::common_think(
      &window,
      &world.m_core_data);
    
    // ** Update World ** //
    
    Application::host_think(
      &world.m_world_data,
      connection,
      delta_time);
    
    uint32_t number_found_with_tag(0);
    Core::Entity_id ids[5];
    
    World_data::world_find_entities_with_tag(&world.m_world_data, Tag::player, &number_found_with_tag, &ids[0], 5);

    
    // ** Graphics ** //

    renderer_nodes.resize(world.m_world_data.entity_pool->size);
    Application::graphics_think(
        &world.m_world_data,
        renderer_nodes.data(),
        renderer_nodes.size(),
        0
      );

    // GUI Test
    {
      // Generate views
      Core::Gui_view view;
      view.set_color(Core::Color::green());
      view.set_envelope(Core::Rect(0, 0, 1, 1));
    }
  }
  
  Network::destroy_connection(connection);
  
  return 0;
}
#endif
#include <application/resources.hpp>
#include <application/entity_factory.hpp>
#include <application/common/ids_object_tags.hpp>

#include <systems/environment/environment.hpp>
#include <systems/transform/transformations.hpp>
#include <systems/network/network.hpp>
#include <systems/network/network_connection.hpp>
#include <systems/entity/generic_id.hpp>

#include <data/data.hpp>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/gui_renderer/gui_renderer.hpp>

#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <math/math.hpp>
#include <utilities/timer.hpp>

#include <iostream>

#include <application/engine/application_client.hpp>
#include <application/engine/application_host.hpp>
#include <application/engine/application_common.hpp>
#include <application/engine/application_graphics.hpp>

#include <systems/audio/audio.hpp>
#include <core/gui/gui_view.hpp>

#include <core/memory/memory.hpp>

namespace
{

#ifdef WIN32
  const bool is_client = true;
#else
  const bool is_client = false;
#endif
}

#ifdef main
#undef main
#endif

#ifndef UNIT_TEST
int
main(int argc, char *argv[])
{
  const size_t chunk_128_mb = 134217728;
  Core::Memory::initialize(chunk_128_mb);
  
  const std::string title = is_client ? "Wired Client" : "Wired Server";

  Environment::Window window;
  Environment::window_create(&window, 800, 480, false, title.c_str());

  sdl::input input;
  input.set_mouse_hold(true);
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  Audio::initialize();
  
  Network::initialize(&std::cout);
  Network::Connection connection;
  
  renderer::initialize(); // TODO: Needs to go for all it does.
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
  Gui_renderer::initialize();
  
  World_data::Model_pool model_pool;
  World_data::model_pool_init(&model_pool);
  
  World_data::Logic_pool logic_pool;
  World_data::logic_pool_init(&logic_pool);
  
  World_data::Texture_pool texture_pool;
  World_data::texture_pool_init(&texture_pool);
  
  World_data::Entity_pool world_entities;
  World_data::entity_pool_init(&world_entities);
  
  World_data::Entity_graph_changes_pool graph_changes;
  World_data::entity_graph_change_pool_init(&graph_changes);
  
  World_data::Camera_pool camera_pool;
  World_data::camera_pool_init(&camera_pool);
  
  World_data::Audio_pool audio_pool;
  World_data::audio_pool_init(&audio_pool);
  
  World_data::Gui_view_pool gui_view_pool;
  World_data::gui_view_pool_init(&gui_view_pool);
  
  World_data::World world_data;
  {
    world_data.audio_pool             = &audio_pool;
    world_data.entity_pool            = &world_entities;
    world_data.entity_graph_changes   = &graph_changes;
    world_data.logic_pool             = &logic_pool;
    world_data.texture_pool           = &texture_pool;
    world_data.camera_pool            = &camera_pool;
    world_data.model_pool             = &model_pool;
    world_data.gui_pool               = &gui_view_pool;
  }
  
  World_data::set_world_data(&world_data);
  
  // Load some audio data
  {
    Audio::Node_sample_2d node;
    node.chunk_to_play = audio_pool.samples[0].chunk;
  }
  
  // Core Data
  Core_data::Core core_data;
  Core_data::Input_pool core_input;
  
  core_data.input_pool = &core_input;

  Core_data::core_data_init(&core_data);
  Core_data::set_core_data(&core_data);

  // Network Data
  Net_data::Net_entity_pool net_entity_pool;
  Net_data::Interpolation_pool interpolation_pool;
  
  Net_data::Network_data net_data;
  net_data.interpolation_pool = &interpolation_pool;
  net_data.net_entity_pool    = &net_entity_pool;

  Net_data::set_net_data(&net_data);
  
  Resource::load_default_resources(&world_data);
  
  if (is_client)
  {
    Application::client_initialize(&connection);
  }
  else
  {
    Application::host_initialize(&world_data, &connection);
    Network::server_create(&connection, &std::cout);
  }
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  
  util::timer frame_timer;
  frame_timer.start();
  
  renderer::clear_color(0.4f, 0.2f, 0.2f);

//  glViewport(0,0,1280, 720);
//  SDL_GL_SetSwapInterval(0);

  // Foop
  bool run_game = true;
  
  while(run_game)
  {
    const float delta_time = static_cast<float>(frame_timer.split()) / 1000.f;

    // ** Common ** //
    
    Application::common_think(
      &window,
      &core_data);
    
    // ** Update World ** //
    
    if(!is_client)
    {
      Application::host_think(
        &world_data,
        &connection,
        delta_time);
      
      uint32_t number_found_with_tag(0);
      Core::Entity_id ids[5];
      
      World_data::world_find_entities_with_tag(&world_data, Tag::player, &number_found_with_tag, &ids[0], 5);
    }
    else
    {
      Application::client_think(
        &world_data,
        &connection,
        delta_time);
      
      uint32_t number_found_with_tag(0);
      Core::Entity_id ids[5];
      
      World_data::world_find_entities_with_tag(&world_data, Tag::network_player, &number_found_with_tag, &ids[0], 5);
    }
    
    // ** Graphics ** //

    renderer_nodes.resize(world_entities.size);
    Application::graphics_think(
        &world_data,
        renderer_nodes.data(),
        renderer_nodes.size(),
        (is_client ? 1 : 0)
      );

    // GUI Test
    {
      // Generate views
      Core::Gui_view view;
      view.set_color(Core::Color::green());
      view.set_envelope(Core::Rect(0, 0, 1, 1));
      
      // Create ndoes
      std::vector<Gui_renderer::Node> nodes;
      
      for(uint32_t i = 0; i < world_data.gui_pool->size; ++i)
      {
        if(world_data.gui_pool->id[i] > 0)
        {
          nodes.push_back(world_data.gui_pool->gui[i]);
        }
      }
    
      //renderer::clear(false, true);
      //Gui_renderer::render_gui_nodes(&nodes[0], nodes.size(), 600, 480);
    }
  }
  
  Network::destroy_connection(&connection);
  
  return 0;
}
#endif
#include <application/resources.hpp>
#include <application/entity_factory.hpp>
#include <application/common/ids_object_tags.hpp>

#include <systems/environment/environment.hpp>
#include <systems/transform/transform.hpp>
#include <systems/network/network.hpp>
#include <systems/network/network_connection.hpp>
#include <systems/physics/physics.hpp>
#include <systems/entity/generic_id.hpp>

#include <data/data.hpp>
#include <data/core_data/core_data.hpp>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>

#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <math/math.hpp>
#include <utils/timer.hpp>

#include <iostream>

#include <application/engine/application_client.hpp>
#include <application/engine/application_host.hpp>
#include <application/engine/application_common.hpp>
#include <application/engine/application_graphics.hpp>


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

int
main(int argc, char *argv[])
{
  const std::string title = is_client ? "Wired Client" : "Wired Server";

  Environment::Window window;
  Environment::Input input_devices;
  memset(&input_devices, 0, sizeof(input_devices));
  Environment::window_create(&window, 800, 480, false, title.c_str());

  sdl::input input;

  input.set_mouse_hold(true);
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  Network::initialize(&std::cout);
  Network::Connection connection;
  
  renderer::initialize();
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
  
  Physics::World phy_world;
  Physics::world_init(&phy_world);
  
  Data::Model_pool model_pool;
  Data::model_pool_init(&model_pool);
  
  Data::Rigidbody_pool rigidbody_pool;
  Data::rigidbody_pool_init(&rigidbody_pool);
  
  Data::Logic_pool logic_pool;
  Data::logic_pool_init(&logic_pool);
  
  Data::Texture_pool texture_pool;
  Data::texture_pool_init(&texture_pool);
  
  Data::Entity_pool world_entities;
  Data::entity_pool_init(&world_entities);
  
  Data::Entity_graph_changes_pool graph_changes;
  Data::entity_graph_change_pool_init(&graph_changes);
  
  Data::Camera_pool camera_pool;
  Data::camera_pool_init(&camera_pool);
  
  Data::World world_data;
  {
    world_data.entity_pool            = &world_entities;
    world_data.entity_graph_changes   = &graph_changes;
    world_data.logic_pool             = &logic_pool;
    world_data.rigidbody_pool         = &rigidbody_pool;
    world_data.texture_pool           = &texture_pool;
    world_data.camera_pool            = &camera_pool;
    world_data.model_pool             = &model_pool;
    world_data.physics_world          = &phy_world;
  }
  
  Data::set_world_data(&world_data);
  
  // Core Data
  Core_data::Core core_data;
  Core_data::Input_data core_input;
  
  core_data.input_data = &core_input;

  Core_data::core_data_init(&core_data);
  Core_data::set_core_data(&core_data);
  
  Resource::load_default_resources(&texture_pool, texture_pool.size, &model_pool, model_pool.size);
  
  if (is_client)
  {
    Application::client_initialize(&connection);
  }
  else
  {
    Application::host_initialize(&world_data, &connection);
    Network::server_create(&connection, &std::cout);
  }

  Physics::world_step(&phy_world, 0.f);
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  renderer_nodes.resize(world_entities.size);
  
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
      &input_devices,
      &core_data);
    
    // ** Update World ** //
    
    if(!is_client)
    {
      Application::host_think(
        &world_data,
        &connection,
        &input_devices,
        delta_time);
      
      size_t number_found_with_tag(0);
      Core::Entity_id ids[5];
      
      Data::world_find_entities_with_tag(&world_data, Tag::player, &number_found_with_tag, &ids[0], 5);
      Data::camera_pool_set_priority(world_data.camera_pool, ids[0], 1);
    }
    else
    {
      Application::client_think(
        &world_data,
        &connection,
        &input_devices,
        delta_time);
      
      size_t number_found_with_tag(0);
      Core::Entity_id ids[5];
      
      Data::world_find_entities_with_tag(&world_data, Tag::network_player, &number_found_with_tag, &ids[0], 5);
      Data::camera_pool_set_priority(world_data.camera_pool, ids[0], 1);
    }
    
    // ** Graphics ** //

    Application::graphics_think(
        &world_data,
        renderer_nodes.data(),
        renderer_nodes.size()
      );
  }
  
  Network::destroy_connection(&connection);
  
  return 0;
}
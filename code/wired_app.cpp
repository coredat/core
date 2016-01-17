#include <application/resources.hpp>
#include <application/entity_factory.hpp>
#include <application/game_logic/actor_local_player.hpp>
#include <application/game_logic/actor_network_player.hpp>

#include <systems/environment/environment.hpp>
#include <systems/transform/transform.hpp>
#include <systems/network/network.hpp>
#include <systems/network/network_connection.hpp>
#include <systems/physics/physics.hpp>
#include <systems/entity/generic_id.hpp>

#include <data/data.hpp>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>

#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <math/math.hpp>
#include <utils/timer.hpp>

#include <iostream>

#include <data/actor/actor.hpp> // what to do with this.

#include <application/application_client.hpp>
#include <application/application_host.hpp>
#include <application/application_common.hpp>
#include <application/application_graphics.hpp>


namespace
{
  const math::mat4  proj = math::mat4_projection(800, 480, 0.1, 1000, math::quart_tau() * 0.6f);

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

//  sdl::window window(800, 480, false, title);
//  sdl::ogl_context ogl(window);

  Environment::Window window;
  Environment::Input input_devices;
  Environment::window_create(&window, 800, 480, false, title.c_str());

  sdl::input input;

  input.set_mouse_hold(true);
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  Network::initialize(&std::cout);
  Network::Connection connection;
  
  if(is_client)
  {
    Network::client_create(&connection, &std::cout);
    //Network::client_connect_to_server(&connection, "92.239.13.99", 6112, 15000, &std::cout);
    Network::client_connect_to_server(&connection, "192.168.0.8", 1234, 5000, &std::cout);
    //Network::client_connect_to_server(&connection, "127.0.0.1", 6112, 5000, &std::cout);
  }
  else
  {
    Network::server_create(&connection, &std::cout);
  }
  
  renderer::initialize();
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
  
  Physics::World phy_world;
  Physics::world_init(&phy_world);
  
  Data::Model_pool model_pool;
  Data::model_pool_init(&model_pool);
  
  Data::Pending_rigidbody_pool rigidbody_loading_pool;
  Data::pending_rigidbody_pool_init(&rigidbody_loading_pool);
  
  Data::Logic_pool logic_pool;
  Data::logic_pool_init(&logic_pool);
  
  Data::Texture_pool texture_pool;
  Data::texture_pool_init(&texture_pool);
  
  Data::Entity_pool world_entities;
  Data::entity_pool_init(&world_entities);
  
  Resource::load_default_resources(&texture_pool, texture_pool.size, &model_pool, model_pool.size);
  
  Entity_factory::create_ground(&world_entities, &rigidbody_loading_pool, &model_pool, &texture_pool);
  //Entity::Entity_id actor_entity = Entity_factory::create_actor(&world_entities, &model_pool, &texture_pool);

  Entity::Entity_id kine_actor_network;
  Entity::Entity_id kine_actor_local;

  {
    kine_actor_local = Entity_factory::create_kinematic_actor(&world_entities, &rigidbody_loading_pool, &model_pool, &texture_pool);
    kine_actor_network = Entity_factory::create_kinematic_actor(&world_entities, &rigidbody_loading_pool, &model_pool, &texture_pool);
  }
  
  // Game Logic
  {
    const auto free_slot = Data::logic_pool_get_slot(&logic_pool);
    new(free_slot) Actor_local_player();
    
    auto base = reinterpret_cast<Logic::Base*>(&logic_pool.storage[0 * logic_pool.storage_size]);
    base->set_entity(kine_actor_local);
    base->set_entity_data(&world_entities);
    base->set_physics_data(&phy_world);
    base->model_pool = &model_pool;
    base->texture_pool = &texture_pool;
    base->pending_rbs = &rigidbody_loading_pool;
  }
  
  {
    const auto free_slot = Data::logic_pool_get_slot(&logic_pool);
    new(free_slot) Actor_network_player();
    
    auto base = reinterpret_cast<Logic::Base*>(&logic_pool.storage[1 * logic_pool.storage_size]);
    base->set_entity(kine_actor_network);
    base->set_entity_data(&world_entities);
    base->set_physics_data(&phy_world);
    base->model_pool = &model_pool;
    base->texture_pool = &texture_pool;
    base->pending_rbs = &rigidbody_loading_pool;
  }
  
  for(auto &obj : logic_pool.objects_in_use)
  {
    reinterpret_cast<Logic::Base*>(obj)->on_start(); // TODO: reinter_cast?
  }
  
  Entity_factory::create_connection_node(&world_entities, &rigidbody_loading_pool, &model_pool, &texture_pool);
  
  const uint32_t number_of_random_cubes = 0;
  for(uint32_t i = 0; i < number_of_random_cubes; ++i)
  {
    Entity_factory::create_random_cube(&world_entities, &rigidbody_loading_pool, &model_pool, &texture_pool);
  }
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  renderer_nodes.resize(world_entities.size);
  
  util::timer frame_timer;
  frame_timer.start();
  
  renderer::clear_color(0.4f, 0.2f, 0.2f);

  //glViewport(0,0,1280, 720);

  // Foop
  bool run_game = true;
  
  while(run_game)
  {
    const float delta_time = static_cast<float>(frame_timer.split()) / 1000.f;
  
    // ** Common ** //
    
    Application::common_think(
      &window,
      &input_devices);
    
    // ** Update World ** //
    
    // Temp while paramaters change
    if(!is_client)
    {
      Application::host_think(
        &world_entities,
        &logic_pool,
        &rigidbody_loading_pool,
        &phy_world,
        &connection,
        &input_devices,
        delta_time);
    }
    else
    {
        Application::client_think(
          &world_entities,
          &logic_pool,
          &rigidbody_loading_pool,
          &phy_world,
          &connection,
          &input_devices,
          delta_time);  
    }
    
    // ** Graphics ** //

    Application::graphics_think(
        &world_entities,
        &texture_pool,
        &model_pool,
        renderer_nodes.data(),
        renderer_nodes.size()
      );
  }
  
  Network::destroy_connection(&connection);
  
  return 0;
}
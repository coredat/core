#include <application/resources.hpp>

#include <systems/network/network.hpp>
#include <systems/network/network_connection.hpp>
#include <systems/model_manager/model_manager.hpp>
#include <systems/texture_manager/texture_manager.hpp>
#include <systems/physics/physics.hpp>

#include <systems/entity/generic_id.hpp>
#include <data/texture_pool.hpp>
#include <data/model_pool.hpp>
#include <data/entity_pool.hpp>

#include <systems/entity/entity_data.hpp> // make into pool like others

#include <iostream>
#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <utils/directory.hpp>
#include <utils/obj_model_loader.hpp>
#include <math/math.hpp>
#include <systems/transform/transform.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <utils/timer.hpp>
#include <renderer/renderer.hpp>
#include "entity_factory.hpp"
#include <data/actor/actor.hpp>



namespace
{
  const math::mat4  proj      = math::mat4_projection(800, 480, 0.1, 1000, math::quart_tau() * 0.6f);
  const bool        is_client = false;
}


int
main()
{
  sdl::window window(800, 480, false, "Wired");
  sdl::ogl_context ogl(window);
  sdl::input input;

  input.set_mouse_hold(true);
  
  std::cout << "Wired" << std::endl;
  
  renderer::set_log_callback([](const int32_t id, const std::string &msg)
  {
    std::cout << id << " - " << msg << std::endl;
  });
  
  Network::Connection connection;
  Network::initialize(&std::cout);
  
  if(is_client)
  {
    Network::client_create(&connection, &std::cout);
    Network::client_connect_to_server(&connection, "127.0.0.1", 1234, 5000, &std::cout);
  }
  else
  {
    Network::server_create(&connection, &std::cout);
  }
  
  renderer::initialize();
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
  
  // Load resources
  Data::Model_pool model_pool;
  Data::Texture_pool texture_pool;
  Resource::load_default_resources(&texture_pool, texture_pool.size, &model_pool, model_pool.size);
  
  Data::Entity_pool world_entities(1024);
  Physics::World phy_world;
  Physics::world_init(&phy_world);
  
  
  Entity_factory::create_ground(&world_entities, &model_pool, &texture_pool);
//  Entity_id actor_entity = Entity_factory::create_actor(&world_entities, &model_pool, &texture_pool);
  Entity::Entity_id kine_actor_local = Entity_factory::create_kinematic_actor(&world_entities, &model_pool, &texture_pool);
  Entity::Entity_id kine_actor_network = Entity_factory::create_kinematic_actor(&world_entities, &model_pool, &texture_pool);
  
  
//  for(int i = 0; i < 1; ++i)
//  {
//    auto ent = Entity_factory::create_random_cube(&world_entities, &mesh_data, nullptr);
//    world_entities.get_texture_data()[i + 2]  = texture_data.tex[1];
//    world_entities.get_mesh_data()[i + 2]     = mesh_data.vbo[0];
//  }
  
  Physics::world_add_rigidbodies(&phy_world,
                                 world_entities.get_collider_data(),
                                 world_entities.size(),
                                 world_entities.get_rigidbody_data(),
                                 world_entities.size());
  
  // Transform data
  std::vector<Simple_renderer::Node> renderer_nodes;
  renderer_nodes.resize(world_entities.size());

  // Copy vbo's into node. *hurt*
  for(std::size_t i = 0; i < world_entities.size(); ++i)
  {
    renderer_nodes.at(i).vbo = world_entities.get_mesh_data()[i];
    renderer_nodes.at(i).diffuse_id = world_entities.get_texture_data()[i];
  }
  
  util::timer frame_timer;
  frame_timer.start();
  
  // Foop
  while(!window.wants_to_quit())
  {
    const float delta_time = static_cast<float>(frame_timer.split()) / 1000.f;
  
    struct Network_transform
    {
      float position[3];
      float rotation[4];
      float scale[3];
    };
  
    Network::poll_events(&connection,
    0,
    [&](const Network::Event_id id, const void *data, const std::size_t size_of_data)
    {
//      const Network_transform *transform = reinterpret_cast<const Network_transform*>(data);
//      const auto index = world_entities.find_index(kine_actor_network);
//      
//      math::transform trans = math::transform_init(
//        math::vec3_init(transform->position[0], transform->position[1], transform->position[2]),
//        math::vec3_init(transform->scale[0], transform->scale[1], transform->scale[2]),
//        math::quat_init(transform->rotation[0], transform->rotation[1], transform->rotation[2], transform->rotation[3])
//      );
//      
//      world_entities.get_transform_data()[index] = trans;
      const Actor::Input_cmds *cmds = reinterpret_cast<const Actor::Input_cmds*>(data);
      Actor::input(*cmds, delta_time, kine_actor_network, &world_entities, world_entities.size(), &phy_world);
    },
    &std::cout);
    
    Network_transform net_trans;
    const auto index = world_entities.find_index(kine_actor_local);
    const math::transform trans = world_entities.get_transform_data()[index];
    
    math::vec3_to_array(trans.position, net_trans.position);
    math::vec3_to_array(trans.scale, net_trans.scale);
    math::quat_to_array(trans.rotation, net_trans.rotation);
    
    //Network::send_packet(&connection, sizeof(net_trans), (void*)&net_trans, false);
  
    sdl::message_pump();
    renderer::clear();
    
    Actor::update(kine_actor_local,   &world_entities, world_entities.size(), &phy_world);
    Actor::update(kine_actor_network, &world_entities, world_entities.size(), &phy_world);
    
    Actor::Input_cmds input_cmds;
    if(input.is_key_down(SDLK_w))
    {
      input_cmds.fwd++;
    }

    if(input.is_key_down(SDLK_s))
    {
      input_cmds.fwd--;
    }
    
    if(input.is_key_down(SDLK_a))
    {
      input_cmds.right++;
    }
    
    if(input.is_key_down(SDLK_d))
    {
      input_cmds.right--;
    }
    
    input_cmds.rot = input.get_mouse_delta_x();
    input_cmds.pitch = input.get_mouse_delta_y();
    
    if(is_client)
    {
      Network::send_packet(&connection, sizeof(input_cmds), &input_cmds, false);
    }
    else
    {
      Actor::input(input_cmds, delta_time, kine_actor_local, &world_entities, world_entities.size(), &phy_world);
    }
    
//    apply_gravity(kine_actor_local);
//    local_controls(kine_actor_local);
    
    Physics::world_step(&phy_world, delta_time);
    
    static float time = 4;
    //time += delta_time / 2;
    
    const float x = math::sin(time) * 9;
    const float z = math::cos(time) * 9;
    
    const float eye_pos[3] = {x, 3.4f, z};
    
    const math::mat4 view  = math::mat4_lookat(math::vec3_init(x, 3.4f, z),
                                               math::vec3_zero(),
                                               math::vec3_init(0, 1, 0));
    
    const math::mat4 view_proj = math::mat4_multiply(view, proj); // *hurt* camaera or such.
    Transform::transforms_to_wvp_mats(world_entities.get_transform_data(),
                                      world_entities.size(),
                                      view_proj,
                                      renderer_nodes[0].wvp,
                                      renderer_nodes.size(),
                                      sizeof(Simple_renderer::Node));

    Transform::transforms_to_world_mats(world_entities.get_transform_data(),
                                        world_entities.size(),
                                        renderer_nodes[0].world_mat,
                                        renderer_nodes.size(),
                                        sizeof(Simple_renderer::Node));
      
    Simple_renderer::render_nodes_fullbright(renderer_nodes.data(), renderer_nodes.size());
    //Simple_renderer::render_nodes_directional_light(renderer_nodes.data(), renderer_nodes.size(), &eye_pos[0]);
    
    //renderer::clear(false, true);
    
    math::mat4 wvp = math::mat4_multiply(math::mat4_id(), view, proj);
    Debug_line_renderer::render(math::mat4_get_data(wvp));
    
    input.think();
    window.flip_buffer();
  }
  
  
  Network::destroy_connection(&connection);
  
  return 0;
}
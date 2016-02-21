#include "application_host.hpp"
#include "../entity_factory.hpp"
#include <data/data.hpp>
#include <systems/network/network.hpp>
#include <core/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>
#include <application/game_logic/actor_model.hpp>
#include <core/interface/entity.hpp>
#include <data/network_data/net_entity_pool.hpp>
#include <data/core_data/input_pool.hpp>
#include <iostream>


namespace
{
  Core::Entity_id kine_actor_local   = Core::Entity_id_util::invalid_id();
  Core::Entity_id kine_actor_network = Core::Entity_id_util::invalid_id();
}


namespace Application {


void
host_initialize(
  World_data::World *world,
  Network::Connection *connection)
{
  {
    kine_actor_local   = Entity_factory::create_local_kinematic_actor(world).get_id();
    kine_actor_network = Entity_factory::create_network_kinematic_actor(world).get_id();
    
    // Create level geometry.
    {
      math::transform place_transform;
      Core::Entity entity;
    
      Entity_factory::create_ground(world);
      
      const float scale = 5;
      const float place_scale = 4;
      
      // Inner
      place_transform.position = math::vec3_init(5 * place_scale,0,-5);
      place_transform.scale = math::vec3_init(scale, scale * 2, scale);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(5 * place_scale,0,+5);
      place_transform.scale = math::vec3_init(scale, scale * 2, scale);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(5 * place_scale,0,+10);
      place_transform.scale = math::vec3_init(scale, scale, scale * 2);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(5 * place_scale,0,-10);
      place_transform.scale = math::vec3_init(scale, scale, scale * 2);
      Entity_factory::create_static_cube(world, place_transform);
      
      place_transform.position = math::vec3_init(-5 * place_scale,0,+10);
      place_transform.scale = math::vec3_init(scale, scale, scale * 2);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(-5 * place_scale,0,-10);
      place_transform.scale = math::vec3_init(scale, scale, scale * 4);
      Entity_factory::create_static_cube(world, place_transform);
      
      place_transform.position = math::vec3_init(-5,0,5 * place_scale);
      place_transform.scale = math::vec3_init(scale * 2, scale, scale);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(-5,5,6 * place_scale);
      place_transform.scale = math::vec3_init(scale, scale, scale * 4);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(10,5,6 * place_scale);
      place_transform.scale = math::vec3_init(scale, scale, scale * 4);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(+10,0,5 * place_scale);
      place_transform.scale = math::vec3_init(scale * 4, scale, scale);
      Entity_factory::create_static_cube(world, place_transform);
      
      place_transform.position = math::vec3_init(-8,0,-5 * place_scale);
      place_transform.scale = math::vec3_init(scale * 2, scale, scale);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(8,0,-5 * place_scale);
      place_transform.scale = math::vec3_init(scale * 2, scale, scale);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(0,5,-5 * place_scale);
      place_transform.scale = math::vec3_init(scale * 2, scale, scale);
      Entity_factory::create_static_cube(world, place_transform);

      place_transform.position = math::vec3_init(15,5,-5 * place_scale);
      place_transform.scale = math::vec3_init(scale, scale * 2, scale);
      Entity_factory::create_static_cube(world, place_transform);
      
      // Outter
      place_transform.position = math::vec3_init(8 * place_scale,0,0);
      place_transform.scale = math::vec3_init(scale, scale, scale * 4);
      Entity_factory::create_static_cube(world, place_transform);
      
      place_transform.position = math::vec3_init(-8 * place_scale,0,0);
      place_transform.scale = math::vec3_init(scale, scale, scale * 4);
      Entity_factory::create_static_cube(world, place_transform);
      
      place_transform.position = math::vec3_init(0,0,8 * place_scale);
      place_transform.scale = math::vec3_init(scale * 4, scale, scale);
      Entity_factory::create_static_cube(world, place_transform);
      
      place_transform.position = math::vec3_init(0,0,-8 * place_scale);
      place_transform.scale = math::vec3_init(scale * 4, scale, scale);
      Entity_factory::create_static_cube(world, place_transform);
    }
    
    // Create npc's
    {
      Core::Entity entity;
      math::transform place_transform;
    
      entity = Entity_factory::create_npc_actor(world);
      entity = Entity_factory::create_npc_actor(world);
      entity = Entity_factory::create_npc_actor(world);
      entity = Entity_factory::create_npc_actor(world);
      entity = Entity_factory::create_npc_actor(world);
      entity = Entity_factory::create_npc_actor(world);
      entity = Entity_factory::create_npc_actor(world);      
    }
  }
  
  // * Add camera's * //
  {
    Camera::Camera_properties cam_props;
    cam_props.fov             = math::quart_tau() * 0.6f;
    cam_props.near_plane      = 0.1f;
    cam_props.far_plane       = 1000.f;
    cam_props.viewport_width  = 800;
    cam_props.viewport_height = 480;
    
    World_data::camera_pool_add_camera(world->camera_pool, kine_actor_local, cam_props);
    World_data::camera_pool_add_camera(world->camera_pool, kine_actor_network, cam_props);
  }
  
  volatile const uint32_t number_of_random_cubes = 0;
  for(uint32_t i = 0; i < number_of_random_cubes; ++i)
  {
    Entity_factory::create_random_cube(world);
  }
}


void
host_think(
  World_data::World *world,
  Network::Connection *connection,
  const float delta_time)
{
  Physics::world_step(world->physics_world, delta_time);

  Network::poll_events(connection,
    0,
    [&](const Network::Event_id id, const void *data, const std::size_t size_of_data)
  {
    float controller_data[4];
    memcpy(&controller_data[0], data, size_of_data);

    Core_data::Game_controller controller;
    controller.axis[0].x = controller_data[0];
    controller.axis[0].y = controller_data[1];
    controller.axis[1].x = controller_data[2];
    controller.axis[1].y = controller_data[3];
    
    Core_data::input_data_update_controller(Core_data::get_core_data()->input_pool, 1, &controller);
  },
    &std::cout);
  
  std::size_t index;
  Core::Entity_id_util::find_index_linearly(&index, kine_actor_local, world->entity_pool->entity_id, world->entity_pool->size);
  
  // ** Game Logic Update ** //
  World_data::logic_pool_on_start_hook(world->logic_pool);
  World_data::logic_pool_on_update_hook(world->logic_pool, delta_time);
  
  // Push in new phy entities.
  World_data::world_update_scene_graph_changes(world, world->entity_graph_changes);
  World_data::rigidbody_pool_update_scene_graph_changes(world->rigidbody_pool, world, world->entity_graph_changes);
  
  // Reset the entity pool for new changes.
  World_data::entity_graph_change_pool_init(world->entity_graph_changes);

  // Build network entity list
  {
    ALIGN_16(static Net_data::Net_camera_pool net_camera);
    ALIGN_16(static Net_data::Net_entity_pool net_pool);
    
    static uint32_t tick = 0;
    
    net_pool.tick = ++tick;
  
    // TODO This stinks need to change it.
    for(size_t i = 0; i < world->entity_pool->size; ++i)
    {
      net_pool.entities[i].entity_id  = Core::Entity_id_util::convert_entity_to_uint(world->entity_pool->entity_id[i]);
      net_pool.entities[i].transform  = world->entity_pool->transform[i];
      net_pool.entities[i].vbo_id     = world->entity_pool->model[i];
      net_pool.entities[i].mat_id     = world->entity_pool->texture[i];
    }
    
    net_pool.size = world->entity_pool->size;
    
    Network::send_packet(connection, sizeof(net_pool), &net_pool, true);
    
    // TODO Stinky
    for(size_t i = 0; i < world->camera_pool->number_of_cameras; ++i)
    {
      memcpy(net_camera.entity_id, world->camera_pool->entity_id, sizeof(world->camera_pool->entity_id));
      memcpy(net_camera.camera, world->camera_pool->camera, sizeof(world->camera_pool->camera));
      memcpy(net_camera.peer_priority_00, world->camera_pool->peer_priority_00, sizeof(world->camera_pool->peer_priority_00));
      memcpy(net_camera.peer_priority_01, world->camera_pool->peer_priority_01, sizeof(world->camera_pool->peer_priority_01));
      memcpy(net_camera.peer_priority_02, world->camera_pool->peer_priority_02, sizeof(world->camera_pool->peer_priority_02));
      memcpy(net_camera.peer_priority_03, world->camera_pool->peer_priority_03, sizeof(world->camera_pool->peer_priority_03));
    }
    
    Network::send_packet(connection, sizeof(net_camera), &net_camera, true);
  }

}


} // ns
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
      
      const float offset = 6.f;
      
      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(+offset,0,0);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(-offset,0,-4);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(+offset,0,-8);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(-offset,0,-10);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);
      
      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(+offset,0,-14);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(-offset,0,-18);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(+offset,0,-22);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(-offset,0,-26);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(+offset,0,-30);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(-offset,0,-34);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);

      entity = Entity_factory::create_static_cube(world);
      place_transform.position = math::vec3_init(+offset,0,-38);
      place_transform.scale = entity.get_transform().scale;
      entity.set_transform(place_transform);
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
  const Environment::Input *inputs,
  const float delta_time)
{
  Physics::world_step(world->physics_world, delta_time);

  Network::poll_events(connection,
    0,
    [&](const Network::Event_id id, const void *data, const std::size_t size_of_data)
  {
    float controller_data[4];
    memcpy(&controller_data[0], data, size_of_data);

    Core::Entity network_actor;
    World_data::world_find_entity(world, &network_actor, kine_actor_network);
    
    Actor_model *actor = network_actor.get_component<Actor_model>(Component_type::actor);
    assert(actor);
    
    actor->move_forward(controller_data[0]);
    actor->move_left(controller_data[1]);
    actor->turn_left(controller_data[2]);
    actor->look_up(controller_data[3]);
    
    // Apply to network actor.
  
//    const Actor::Input_cmds *cmds = reinterpret_cast<const Actor  ::Input_cmds*>(data);
//    Actor::input(*cmds, delta_time, kine_actor_network, world->entity_pool, world->entity_pool->size, world->physics_world);
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
    static Net_data::Net_entity_pool net_pool;
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
    
    Network::send_packet(connection, sizeof(net_pool), &net_pool, false);
  }

}


} // ns
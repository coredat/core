#include "application_host.hpp"
#include "../entity_factory.hpp"
#include <data/data.hpp>
#include <systems/network/network.hpp>
#include <core/entity_id.hpp>
#include <systems/environment/environment.hpp>
#include <core/interface/entity.hpp>
#include <data/network_data/net_entity_pool.hpp>
#include <data/core_data/input_pool.hpp>
#include <systems/physics_engine/physics_engine.hpp>
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
  // * Add camera's * //
  {
    Camera::Camera_properties cam_props;
    cam_props.fov             = math::quart_tau() * 0.6f;
    cam_props.near_plane      = 0.1f;
    cam_props.far_plane       = 1000.f;
    cam_props.viewport_width  = 800;
    cam_props.viewport_height = 480;
  }
  
  // Create an entity
  {
   
//    Entity_factory::create_ground(world);
    Entity_factory::create_actor(world);
    Entity_factory::create_game_play_camera(world);
    Entity_factory::create_game_state(world);
  
//    Core::Entity test_cube;
//    World_data::world_create_new_entity(world, &test_cube, 1);
//    
//    test_cube.set_transform(Core::Transform(math::vec3_zero(), math::vec3_one(), math::quat_init()));
//    test_cube.set_material_id(1);
//    test_cube.set_model_id(0);
  }
}


void
host_think(
  World_data::World *world,
  Network::Connection *connection,
  const float delta_time)
{
//  Network::poll_events(connection,
//    0,
//    [&](const Network::Event_id id, const void *data, const std::uint32_t size_of_data)
//  {
//    float controller_data[4];
//    memcpy(&controller_data[0], data, size_of_data);
//
//    Core_data::Game_controller controller;
//    controller.axis[0].x = controller_data[0];
//    controller.axis[0].y = controller_data[1];
//    controller.axis[1].x = controller_data[2];
//    controller.axis[1].y = controller_data[3];
//    
//    Core_data::input_data_update_controller(Core_data::get_core_data()->input_pool, 1, &controller);
//  },
//    &std::cout);
  
  std::uint32_t index;
  Core::Entity_id_util::find_index_linearly(&index, kine_actor_local, world->entity_pool->entity_id, world->entity_pool->size);
  
  // ** Run physics ** //
  constexpr uint32_t size_of_pairs = 128;
  Physics_engine::Collision_pair collision_pairs[size_of_pairs];
  
  uint32_t number_of_collisions(0);
  
  Physics_engine::get_collisions(
    world->entity_pool->entity_id,
    world->entity_pool->transform,
    world->entity_pool->aabb,
    world->entity_pool->size,
    collision_pairs,
    size_of_pairs,
    &number_of_collisions
  );
  
  // Alert the collision callbacks
  for(uint32_t i = 0; i < number_of_collisions; ++i)
  {
    World_data::logic_pool_on_collision_hook(world->logic_pool, collision_pairs[i].obj_a, collision_pairs[i].obj_b);
  }
  
  // ** Game Logic Update ** //
  World_data::logic_pool_on_start_hook(world->logic_pool);
  World_data::logic_pool_on_update_hook(world->logic_pool, delta_time);
  
  // Push in new phy entities.
  World_data::world_update_scene_graph_changes(world, world->entity_graph_changes);

  
  // Reset the entity pool for new changes.
  World_data::entity_graph_change_pool_init(world->entity_graph_changes);
  

  // Build network entity list
  {
//    ALIGN_16(static Net_data::Net_camera_pool net_camera);
//    ALIGN_16(static Net_data::Net_entity_pool net_pool);
//    
//    static uint32_t tick = 0;
//    
//    net_pool.tick = ++tick;
//  
//    // TODO This stinks need to change it.
//    for(uint32_t i = 0; i < world->entity_pool->size; ++i)
//    {
//      net_pool.entities[i].entity_id  = Core::Entity_id_util::convert_entity_to_uint(world->entity_pool->entity_id[i]);
//      net_pool.entities[i].transform  = world->entity_pool->transform[i];
//      net_pool.entities[i].vbo_id     = world->entity_pool->model[i];
//      net_pool.entities[i].mat_id     = world->entity_pool->texture[i];
//    }
//    
//    net_pool.size = world->entity_pool->size;
//    
//    Network::send_packet(connection, sizeof(net_pool), &net_pool, true);
//    
//    // TODO Stinky
//    for(uint32_t i = 0; i < world->camera_pool->number_of_cameras; ++i)
//    {
//      memcpy(net_camera.entity_id, world->camera_pool->entity_id, sizeof(world->camera_pool->entity_id));
//      memcpy(net_camera.camera, world->camera_pool->camera, sizeof(world->camera_pool->camera));
//      memcpy(net_camera.peer_priority_00, world->camera_pool->peer_priority_00, sizeof(world->camera_pool->peer_priority_00));
//      memcpy(net_camera.peer_priority_01, world->camera_pool->peer_priority_01, sizeof(world->camera_pool->peer_priority_01));
//      memcpy(net_camera.peer_priority_02, world->camera_pool->peer_priority_02, sizeof(world->camera_pool->peer_priority_02));
//      memcpy(net_camera.peer_priority_03, world->camera_pool->peer_priority_03, sizeof(world->camera_pool->peer_priority_03));
//    }
//    
//    Network::send_packet(connection, sizeof(net_camera), &net_camera, true);
  }

}


} // ns
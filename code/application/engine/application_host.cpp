#include "application_host.hpp"
#include "../entity_factory.hpp"
#include <data/data.hpp>
#include <core/entity_id.hpp>
#include <systems/environment/environment.hpp>
#include <core/interface/entity.hpp>
#include <data/core_data/input_pool.hpp>
#include <systems/physics_engine/physics_engine.hpp>
#include <iostream>


namespace Application {


void
host_initialize(
  World_data::World *world)
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
  
  // Create some entities
  {
//    Entity_factory::create_actor(world);
//    Entity_factory::create_game_play_camera(world);
//    Entity_factory::create_game_state(world);
  }
}


void
host_think(
  World_data::World *world,
  const float delta_time)
{
  // ** Run physics ** //
  constexpr uint32_t size_of_pairs = 2048; // TODO: Alloc mem for this!
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
    World_data::logic_pool_on_collision_hook(world->logic_pool,
                                             collision_pairs[i].obj_a,
                                             collision_pairs[i].obj_b);
  }
  
  // ** Game Logic Update ** //
  World_data::logic_pool_on_start_hook(world->logic_pool);
  World_data::logic_pool_on_early_update_hook(world->logic_pool, delta_time);
  World_data::logic_pool_on_update_hook(world->logic_pool, delta_time);
  
  // Push in new phy entities.
  World_data::world_update_scene_graph_changes(world, world->entity_graph_changes);

  World_data::logic_pool_clean_up(world->logic_pool);
  
  // Reset the entity pool for new changes.
  World_data::entity_graph_change_pool_init(world->entity_graph_changes);
  
}


} // ns
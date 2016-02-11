#include "world.hpp"
#include "world_pools.hpp"
#include <atomic>


namespace
{
  // The global world state.
  World_data::World *curr_world = nullptr;
}


namespace World_data {


void
set_world_data(World *set_world) {
  curr_world = set_world;
}


World*
get_world() {
  return curr_world;
}


Physics::World*
get_physics_world(World *world_data) {
  return world_data->physics_world;
}


} // ns
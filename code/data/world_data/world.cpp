#include "world.hpp"
#include "world_pools.hpp"
#include <utilities/logging.hpp>


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
get_world()
{
//  LOG_TODO("Needs to go!")
  return curr_world;
}


} // ns
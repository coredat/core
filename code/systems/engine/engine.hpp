#ifndef ENGINE_INCLUDED_028C7BEE_E9AA_408E_AEA1_55EF0DDB370F
#define ENGINE_INCLUDED_028C7BEE_E9AA_408E_AEA1_55EF0DDB370F


#include "tick_information.hpp"
#include <data/world_data/world_pools.hpp>
#include <data/global_data/resource_data.hpp>

#include <core/world/world.hpp> // Kill

namespace Engine {


void
initialize();


void
think(const std::shared_ptr<World_data::World> world_data,
      const std::shared_ptr<Resource_data::Resources> resource_data,
      const float dt,
      const float running_time,
      const uint32_t width,
      const uint32_t height,
      Tick_information *out_tick_info = nullptr);


/*
  Temp hack till core can access the collision data specifically.
*/
void
set_collision_callback(Core::Collision_callback callback);


void
de_initialize();


} // ns


#endif // inc guard
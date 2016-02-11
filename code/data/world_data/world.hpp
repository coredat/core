#ifndef WORLD_INCLUDED_AE8FBA1B_3333_49DA_9E90_791E9F75649C
#define WORLD_INCLUDED_AE8FBA1B_3333_49DA_9E90_791E9F75649C


#include "world_data_fwd.hpp"
#include <systems/physics/physics_fwd.hpp>


/*!
  We hold the global state of the current world.
  This isn't great, however we consider this 'ok'
  because holding and accessing multiple worlds isn't a thing.
  And if it is it should be managed by serializing the current one first.
*/
namespace World_data {


/*!
  Sets the world data global.
  \param world the world to set.
*/
void
set_world_data(World *world);


/*!
  Get the current world data.
  \return The current world.
*/
World*
get_world();


/*!
  Get the physics world.
  \param the world whose physics you want to access.
  \return the current physics world.
*/
Physics::World*
get_physics_world(World *world);


} // ns


#endif // inc guard
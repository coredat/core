#ifndef PHYSICS_TICK_INCLUDED_37BBB181_825A_416C_A1E4_13E1DF9736CF
#define PHYSICS_TICK_INCLUDED_37BBB181_825A_416C_A1E4_13E1DF9736CF


#include <data/world_data/world_data.hpp> // fwd


#include <core/world/world.hpp> // for collision callback haack


namespace Engine {
namespace Physics_tick {


void
initialize();


/*
  TODO:
  Break down world to the actual data sources we want.
*/
void
think(std::shared_ptr<World_data::World> world, const float dt);


void
de_initialize();


void
collision_callback(Core::Collision_callback callback);


} // ns
} // ns


#endif // inc guard
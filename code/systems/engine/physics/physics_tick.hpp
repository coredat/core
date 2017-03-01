#ifndef PHYSICS_TICK_INCLUDED_37BBB181_825A_416C_A1E4_13E1DF9736CF
#define PHYSICS_TICK_INCLUDED_37BBB181_825A_416C_A1E4_13E1DF9736CF


#include "../tick_information.hpp"
#include <core/world/world.hpp> // for collision callback haackx
#include <data/fwd_dec_collections.hpp>


namespace Engine {
namespace Physics_tick {


void
initialize(std::shared_ptr<Data::World> world);


void
think(std::shared_ptr<Data::World> world, const float dt, Tick_information *out_tick_info = nullptr);


void
de_initialize();


void
collision_callback(Core::Collision_callback callback);


} // ns
} // ns


#endif // inc guard

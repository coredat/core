#ifndef APPLICATION_HOST_INCLUDED_76BC2CD5_739F_4E98_B584_1DDB9EEA350B
#define APPLICATION_HOST_INCLUDED_76BC2CD5_739F_4E98_B584_1DDB9EEA350B


#include <data/world_data/world_data_fwd.hpp>
#include <systems/environment/environment_fwd.hpp>


namespace Application {


/*!
  Init's the host things.
*/
void
host_initialize(
  World_data::World *world);


/*!
  host_think() is how the game rules are processed.
  The result of these are transfered to the clients.
*/
void
host_think(
  World_data::World *world,
  const float delta_time);


} // ns


#endif // inc guard
#ifndef APPLICATION_HOST_INCLUDED_76BC2CD5_739F_4E98_B584_1DDB9EEA350B
#define APPLICATION_HOST_INCLUDED_76BC2CD5_739F_4E98_B584_1DDB9EEA350B


#include <data/data_fwd.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/network/network_fwd.hpp>
#include <systems/environment/environment_fwd.hpp>


namespace Application {


/*!
  Init's the host things.s
*/
void
host_initialize(
  Data::World *world,
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
  Data::Pending_rigidbody_pool *pending_rbs,
  Data::Model_pool *model_pool,
  Data::Texture_pool *texture_pool,
  Physics::World *phy_world,
  Network::Connection *connection);


/*!
  host_think() is how the game rules are processed.
  The result of these are transfered to the clients.
*/
void
host_think(
  Data::World *world,
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
  Data::Pending_rigidbody_pool *pending_rbs,
  Physics::World *phy_world,
  Network::Connection *connection,
  const Environment::Input *inputs,
  const float delta_time);

}


#endif // inc guard
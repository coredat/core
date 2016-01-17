#ifndef APPLICATION_CLIENT_INCLUDED_
#define APPLICATION_CLIENT_INCLUDED_


#include <data/data_fwd.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/network/network_fwd.hpp>
#include <systems/environment/environment_fwd.hpp>


namespace Application {


/*!
  client_think() process the incoming entity data from the network.
*/
void client_think(
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
  Data::Pending_rigidbody_pool *pending_rbs,
  Physics::World *phy_world,
  Network::Connection *connection,
  const Environment::Input *inputs,
  const float delta_time);


} // ns



#endif // inc guard

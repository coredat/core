#ifndef APPLICATION_CLIENT_INCLUDED_CB73EE7C_41F5_4B66_9B30_6188643D035A
#define APPLICATION_CLIENT_INCLUDED_CB73EE7C_41F5_4B66_9B30_6188643D035A


#include <data/data_fwd.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/network/network_fwd.hpp>
#include <systems/environment/environment_fwd.hpp>


namespace Application {


/*!
 init the client
*/
void
client_initialize(Network::Connection *connection);


/*!
  client_think() process the incoming entity data from the network.
*/
void
client_think(
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
  Data::Pending_rigidbody_pool *pending_rbs,
  Physics::World *phy_world,
  Network::Connection *connection,
  const Environment::Input *inputs,
  const float delta_time);


} // ns


#endif // inc guard

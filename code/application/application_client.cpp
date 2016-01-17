#include "application_client.hpp"
#include <data/entity_pool.hpp>
#include <data/logic_pool.hpp>
#include <data/pending_rigidbody_pool.hpp>
#include <data/actor/actor.hpp> // KILL!!!
#include <systems/network/network.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>


namespace Application {


void
client_think(
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
  Data::Pending_rigidbody_pool *pending_rbs,
  Physics::World *phy_world,
  Network::Connection *connection,
  const Environment::Input *inputs,
  const float delta_time)
{
  Network::poll_events(connection,
    0,
    [&](const Network::Event_id id, const void *data, const std::size_t size_of_data)
    {
        // We just copy entitiy positions into our entity pool.
        // So much todo here!
        memcpy(entity_pool->transform, data, size_of_data);
    },
    &std::cout);

  Actor::Input_cmds input_cmds;
   
  input_cmds.fwd = inputs->controllers[0].axis_2[1];
  input_cmds.right = inputs->controllers[0].axis_2[0];

  input_cmds.rot = inputs->controllers[0].axis_1[0];
  input_cmds.pitch = inputs->controllers[0].axis_1[1];

  Network::send_packet(connection, sizeof(input_cmds), &input_cmds, false);
}


} // ns
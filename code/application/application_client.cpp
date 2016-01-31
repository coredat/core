#include "application_client.hpp"
#include <data/data.hpp>
#include <systems/network/network.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>
#include <iostream>


namespace Application {


void
client_initialize(
  Network::Connection *connection)
{
  const char *server_ip = "192.168.0.8";
  const uint32_t port = 1234;
  const uint32_t timeout = 5000;

  Network::client_create(connection, &std::cout);
  Network::client_connect_to_server(connection, server_ip, port, timeout, &std::cout);
}


void
client_think(
  Data::World *world,
  Data::Entity_pool *entity_pool,
  Data::Logic_pool *logic_pool,
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

//  Actor::Input_cmds input_cmds;
   
//  input_cmds.fwd = inputs->controllers[0].axis_2[1];
//  input_cmds.right = inputs->controllers[0].axis_2[0];

//  input_cmds.rot = inputs->controllers[0].axis_1[0];
//  input_cmds.pitch = inputs->controllers[0].axis_1[1];

//  Network::send_packet(connection, sizeof(input_cmds), &input_cmds, false);
}


} // ns
#include "application_client.hpp"
#include <data/data.hpp>
#include <systems/network/network.hpp>
#include <core/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>
#include <iostream>
#include <core/input/input.hpp>


namespace Application {


void
client_initialize(
  Network::Connection *connection)
{
  const char *server_ip = "192.168.0.10";
  const uint32_t port = 1234;
  const uint32_t timeout = 5000;

  Network::client_create(connection, &std::cout);
  Network::client_connect_to_server(connection, server_ip, port, timeout, &std::cout);
}


void
client_think(
  Data::World *world,
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
        memcpy(world->entity_pool->transform, data, size_of_data);
    },
    &std::cout);

  auto controller = Core::Input::get_controller(Core::Input::Player::one);
  
  uint8_t data[16];
  controller.serialize(data);
  
  float ft_data[4];
  memcpy(&ft_data[0], &data[0], sizeof(data));

  std::cout << ft_data[0] << ", " << ft_data[1] << ", " << ft_data[2] << ", " << ft_data[3] << std::endl;

  Network::send_packet(connection, sizeof(data), &data[0], false);
}


} // ns
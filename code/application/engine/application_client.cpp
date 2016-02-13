#include "application_client.hpp"
#include <data/world_data/world_data.hpp>
#include <systems/network/network.hpp>
#include <core/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>
#include <iostream>
#include <core/input/input.hpp>
#include <data/network_data/net_entity_pool.hpp>
#include <application/resources.hpp>
#include <utils/alignment.hpp>


namespace
{
  
}


namespace Application {


void
client_initialize(
  Network::Connection *connection)
{
  const char *server_ip = "192.168.0.6";
  const uint32_t port = 1234;
  const uint32_t timeout = 5000;

  Network::client_create(connection, &std::cout);
  Network::client_connect_to_server(connection, server_ip, port, timeout, &std::cout);
  
  // * Set camera * //

//  world->camera_pool->entity_id[0] = kine_actor_local;
}


void
client_think(
  World_data::World *world,
  Network::Connection *connection,
  const Environment::Input *inputs,
  const float delta_time)
{

  ALIGN_16(static Net_data::Net_entity_pool incoming_ents);

  Network::poll_events(connection,

    0,
    [&](const Network::Event_id id, const void *data, const size_t size_of_data)
    {
      memcpy(static_cast<void*>(&incoming_ents), data, size_of_data);
    },
    &std::cout);

  // Update the entity pool
  {
    for (size_t i = 0; i < incoming_ents.capacity; ++i)
    {
      size_t s = sizeof(Net_data::Net_entity);
      Core::Entity_id id = Core::Entity_id_util::convert_uint_to_entity(incoming_ents.entities[i].entity_id);
      world->entity_pool->entity_id[i]  = id;
      world->entity_pool->transform[i]  = incoming_ents.entities[i].transform;
      world->entity_pool->model[i]      = (Resource::Model::ENUM)incoming_ents.entities[i].vbo_id;
      world->entity_pool->texture[i]    = (Resource::Texture::ENUM)incoming_ents.entities[i].mat_id;
    }

    world->entity_pool->size = incoming_ents.capacity;
  }

  auto controller = Core::Input::get_controller(Core::Input::Player::one);
  
  uint8_t data[16];
  controller.serialize(data);
  
  float ft_data[4];
  memcpy(&ft_data[0], &data[0], sizeof(data));

  Network::send_packet(connection, sizeof(data), &data[0], false);
}


} // ns
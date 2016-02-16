#include "application_client.hpp"
#include <data/data.hpp>
#include <systems/network/network.hpp>
#include <core/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/environment/environment.hpp>
#include <iostream>
#include <core/input/input.hpp>
#include <application/resources.hpp>
#include <utils/alignment.hpp>


namespace Application {


void
client_initialize(
  Network::Connection *connection)
{
  const char *server_ip   = "192.168.0.6";
  const uint32_t port     = 1234;
  const uint32_t timeout  = 5000;

  Network::client_create(connection, &std::cout);
  Network::client_connect_to_server(connection, server_ip, port, timeout, &std::cout);
  
  // * Set camera * //

//  world->camera_pool->entity_id[0] = kine_actor_local;
}


void
client_think(
  World_data::World *world,
  Network::Connection *connection,
  const float delta_time)
{
  //ALIGN_16(static Net_data::Net_entity_pool incoming_ents);
  auto inter_pool = Net_data::get_net_data()->interpolation_pool;

  Network::poll_events(connection,
    0,
    [&](const Network::Event_id id, const void *data, const size_t size_of_data)
    {
      const Net_data::Net_entity_pool *peek_pool = reinterpret_cast<const Net_data::Net_entity_pool *>(data);

      // If tick is newer than the ones we have then lets keep it.
      
      if(inter_pool->last_tick < peek_pool->tick)
      {
        inter_pool->last_tick = peek_pool->tick;
        
        memcpy(&inter_pool->snapshot[++(inter_pool->rotate_point) % inter_pool->max_snapshots], data, size_of_data);
      }
    },
    &std::cout);

  // Update the entity pool
  {
    if(inter_pool->rotate_point > 0)
    {
      auto incoming_ents = inter_pool->snapshot[inter_pool->rotate_point % inter_pool->max_snapshots];

      for (size_t i = 0; i < incoming_ents.capacity; ++i)
      {
        size_t s = sizeof(Net_data::Net_entity);
        Core::Entity_id id = Core::Entity_id_util::convert_uint_to_entity(incoming_ents.entities[i].entity_id);
        world->entity_pool->entity_id[i]  = id;
        world->entity_pool->model[i]      = (Resource::Model::ENUM)incoming_ents.entities[i].vbo_id;
        world->entity_pool->texture[i]    = (Resource::Texture::ENUM)incoming_ents.entities[i].mat_id;

        // Interpolate the position
        {
          // Block copy the transform.
          world->entity_pool->transform[i] = incoming_ents.entities[i].transform;

          auto herm_inter = [](const float mu,
                               const float point_start,
                               const float point_end,
                               const float direction_start,
                               const float direction_end)
          {
            const float mu_sq = mu * mu;
            const float mu_cu = mu_sq * mu;

            const float h1 = +2 * mu_cu - 3 * mu_sq + 1;
            const float h2 = -2 * mu_cu + 3 * mu_sq;
            const float h3 = mu_cu - 2 * mu_sq + mu;
            const float h4 = mu_cu - mu_sq;


          };
          
        }
      }

      // Need to sort this.
      world->entity_pool->size = incoming_ents.capacity;
    }
  }

  auto controller = Core::Input::get_controller(Core::Input::Player::one);
  
  uint8_t data[16];
  controller.serialize(data);
  
  float ft_data[4];
  memcpy(&ft_data[0], &data[0], sizeof(data));

  Network::send_packet(connection, sizeof(data), &data[0], false);
}


} // ns
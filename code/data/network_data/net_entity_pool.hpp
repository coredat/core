#ifndef NETWORK_ENTITY_POOL_INCLUDED_16084A2F_89B0_4CB4_B0DD_19F4A95D504E
#define NETWORK_ENTITY_POOL_INCLUDED_16084A2F_89B0_4CB4_B0DD_19F4A95D504E


#include <stdint.h>
#include <math/transform/transform.hpp>
#include <utilities/alignment.hpp>

#include <core/entity_id.hpp>
#include <systems/camera/camera_properties.hpp>


#define NET_ENTITY_POOL_SIZE 128


namespace Net_data {


/*!
  Entity pack we are going to send/recive accorss network.
*/
PACK(struct Net_entity
{
  uint32_t        entity_id;
  uint32_t        vbo_id;
  uint32_t        mat_id;
  math::transform transform;
});


/*!
  Entity collection that gets sent to clients.
*/
PACK(struct Net_entity_pool
{
  uint32_t          type_id = 0;
  Net_entity        entities[NET_ENTITY_POOL_SIZE];
  uint32_t          tick = 0;
  const uint16_t    capacity = NET_ENTITY_POOL_SIZE;
  uint16_t          size = 0;
});


PACK(struct Net_camera_pool
{
  uint32_t         type_id = 1;
  Core::Entity_id             entity_id[10];
  Camera::Camera_properties   camera[10];
  uint32_t                    peer_priority_00[10];
  uint32_t                    peer_priority_01[10];
  uint32_t                    peer_priority_02[10];
  uint32_t                    peer_priority_03[10];
});


void
net_entity_pool_init(Net_entity_pool *pool);


} // ns


#endif // inc guard
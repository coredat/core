#ifndef NETWORK_ENTITY_POOL_INCLUDED_16084A2F_89B0_4CB4_B0DD_19F4A95D504E
#define NETWORK_ENTITY_POOL_INCLUDED_16084A2F_89B0_4CB4_B0DD_19F4A95D504E


#include <stdint.h>
#include <math/transform/transform.hpp>
#include <utils/alignment.hpp>


#define NET_ENTITY_POOL_SIZE 128


namespace Net_data {


PACK(struct Net_entity
{
  uint32_t    entity_id;
  uint32_t    vbo_id;
  uint32_t    mat_id;
  math::transform transform;
});


PACK(struct Net_entity_pool
{
  Net_entity        entities[NET_ENTITY_POOL_SIZE];
  uint32_t          tick = 0;
  uint16_t          capacity = NET_ENTITY_POOL_SIZE;
});


void
net_entity_pool_init(Net_entity_pool *pool);


} // ns


#endif // inc guard
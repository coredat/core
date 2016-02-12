#ifndef NETWORK_ENTITY_POOL_INCLUDED_16084A2F_89B0_4CB4_B0DD_19F4A95D504E
#define NETWORK_ENTITY_POOL_INCLUDED_16084A2F_89B0_4CB4_B0DD_19F4A95D504E


#include <stddef.h>
#include <stdint.h>


#define NET_ENTITY_POOL_SIZE 512


namespace Net_data {


struct Net_entity
{
  uint32_t  entity_id;
  float     position[3];
  float     scale[3];
  float     rotation[4];
};


struct Net_entity_pool
{
  Net_entity        entities[NET_ENTITY_POOL_SIZE];
  size_t            sequence = 0;
  const size_t      capacity = NET_ENTITY_POOL_SIZE;
};


void
net_entity_pool_init(Net_entity_pool *pool);


} // ns


#endif // inc guard
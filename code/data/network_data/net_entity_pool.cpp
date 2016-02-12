#include "net_entity_pool.hpp"
#include <cstring>


namespace Net_data {


void
net_entity_pool_init(Net_entity_pool *pool)
{
  memset(pool->entities, 0, sizeof(pool->entities));
  pool->sequence = 0;
}


} // ns
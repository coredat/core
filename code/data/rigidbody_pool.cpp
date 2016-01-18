#include "rigidbody_pool.hpp"


namespace Data {


void
rigidbody_pool_init(Rigidbody_pool *pool)
{
  memset(pool->entity_id, 0, sizeof(pool->entity_id));
  memset(pool->rigidbody, 0, sizeof(pool->rigidbody));
}


} // ns
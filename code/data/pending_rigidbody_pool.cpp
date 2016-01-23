#include "pending_rigidbody_pool.hpp"
#include "world_data.hpp"
#include <cstring>
#include <assert.h>



namespace Data {


void
pending_rigidbody_pool_init(Pending_rigidbody_pool *pool)
{
  assert(pool);

//  memset(pool->rigidbody_out, 0, sizeof(pool->rigidbody_out));
//  memset(pool->rigidbody_property, 0, sizeof(pool->rigidbody_property));
//  memset(pool->rigidbody_collider, 0, sizeof(pool->rigidbody_collider));
  pool->size = 0;
}


void
pending_rigidbody_pool_clear(Pending_rigidbody_pool *pool)
{
  pending_rigidbody_pool_init(pool);
}


} // ns
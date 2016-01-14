#include "pending_rigidbody_pool.hpp"
#include <cstring>
#include <assert.h>



namespace Data {


void
pending_rigidbody_pool_init(Pending_rigidbody_pool *pool)
{
  assert(pool);

  memset(pool->rigidbody_out, 0, sizeof(pool->rigidbody_out));
  memset(pool->rigidbody_property, 0, sizeof(pool->rigidbody_property));
  pool->size = 0;
}


void
pending_rigidbody_pool_push(Pending_rigidbody_pool *pool, const Physics::Rigidbody_properties props, Physics::Rigidbody *output)
{
  const std::size_t size = pool->size;

  assert(size < pool->capacity);
  
  if(pool->size >= pool->capacity)
  {
    return;
  }

  pool->rigidbody_out[size] = output;
  pool->rigidbody_property[size] = props;
  
  pool->size = size + 1;
}


void
pending_rigidbody_pool_clear(Pending_rigidbody_pool *pool)
{
  pending_rigidbody_pool_init(pool);
}


} // ns
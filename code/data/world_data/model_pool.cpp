#include "model_pool.hpp"


namespace World_data {


void
model_pool_init(Model_pool *pool)
{
  memset(pool->id, 0, sizeof(pool->id));
  memset(pool->vbo, 0, sizeof(pool->vbo));
}


} // ns
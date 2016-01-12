#include "logic_pool.hpp"
#include <cstring>


namespace Data {


void
logic_pool_init(Logic_pool *pool)
{
  memset(pool->start_ups, 0, sizeof(pool->start_ups));
  memset(pool->updates, 0, sizeof(pool->updates));
  memset(pool->storage, 0, sizeof(pool->storage));
  
  // Create free list
  for(std::size_t i = 0; i < pool->size; ++i)
  {
    pool->free_list[i] = &pool->storage[i * pool->storage_size];
  }
}






} // ns
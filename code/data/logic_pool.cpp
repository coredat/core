#include "logic_pool.hpp"
#include <cstring>


namespace Data {


void
logic_pool_init(Logic_pool *pool)
{
  memset(pool->storage, 0, sizeof(pool->storage));
  
  pool->free_list.resize(pool->size);
  
  // Create free list
  for(std::size_t i = 0; i < pool->size; ++i)
  {
    pool->free_list[i] = &pool->storage[i * pool->storage_size];
  }
}


void*
logic_pool_get_slot(Logic_pool *pool)
{
  // TODO: Oh my this needs some sorting.
  void *slot = pool->free_list[0];
  
  pool->free_list.erase(std::begin(pool->free_list));
  pool->objects_in_use.push_back(slot);
  
  return slot;
}



} // ns
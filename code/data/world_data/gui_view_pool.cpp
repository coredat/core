#include "gui_view_pool.hpp"
#include <cstring>


namespace World_data {


void
gui_view_pool_init(Gui_view_pool *pool)
{
  memset(pool->id, 0, sizeof(pool->id));
  memset(pool->gui, 0, sizeof(pool->gui));
}



bool
gui_view_pool_add(Gui_view_pool *pool, const uint32_t id)
{
  // Find a free slot
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->id[i] == 0)
    {
      pool->id[i] = id;
      return true;
    }
  }
  
  return false;
}


void
gui_view_pool_delete(Gui_view_pool *pool, const uint32_t id)
{
  // Find a free slot
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->id[i] == id)
    {
      pool->id[i] = 0;
    }
  }
}


void
gui_view_pool_get(const Gui_view_pool *pool, const uint32_t id, Gui_renderer::Node *out_view)
{
  // Find a free slot
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->id[i] == id)
    {
      memcpy((void*)out_view, &pool->gui[i], sizeof(Gui_renderer::Node));
    }
  }
}


void
gui_view_pool_set(const Gui_view_pool *pool, const uint32_t id, const Gui_renderer::Node *udpate_view)
{
  // Find a free slot
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->id[i] == id)
    {
      memcpy((void*)&pool->gui[i], udpate_view, sizeof(Gui_renderer::Node));
    }
  }
}


} // ns
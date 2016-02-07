#include "camera_pool.hpp"
#include <cstring>
#include <assert.h>


namespace Data {


void
camera_pool_init(Camera_pool *pool)
{
  memset(&pool->entity_id[0], 0, sizeof(pool->entity_id));
  memset(&pool->camera[0], 0, sizeof(pool->camera));
  memset(&pool->priority[0], 0, sizeof(pool->priority));
}


void
camera_pool_add_camera(Camera_pool *pool,
                       const Core::Entity_id id,
                       const Camera::Camera_properties props)
{
  for(size_t i = 0; i < pool->number_of_cameras; ++i)
  {
    if(pool->entity_id[i] == Core::Entity_id_util::invalid_id())
    {
      pool->entity_id[i] = id;
      pool->camera[i] = props;
    }
  }
}


void
camera_pool_set_priority(Camera_pool *pool,
                         const Core::Entity_id id,
                         const uint32_t priority)
{
  size_t index;

  if(Core::Entity_id_util::find_index_linearly(&index,
                                               id,
                                               pool->entity_id,
                                               pool->number_of_cameras))
  {
    pool->priority[index] = priority;
  }
}


Camera::Camera_properties
camera_pool_get_properties_for_priority(Camera_pool *pool,
                         const uint32_t priority)
{
  assert(pool);
  
  for(size_t i = 0; i < pool->number_of_cameras; ++i)
  {
    if(pool->priority[i] == priority)
    {
      return pool->camera[i];
    }
  }
  
  // Failed
  return Camera::Camera_properties{};
}


Core::Entity_id
camera_pool_get_entity_id_for_priority(Camera_pool *pool,
                                       const uint32_t priority)
{
  assert(pool);

  for (size_t i = 0; i < pool->number_of_cameras; ++i)
  {
    if (pool->priority[i] == priority)
    {
      return pool->entity_id[i];
    }
  }

  // Failed
  return Core::Entity_id_util::invalid_id();
}


} // ns
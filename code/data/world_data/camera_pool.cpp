#include "camera_pool.hpp"
#include <cstring>
#include <assert.h>


namespace World_data {


void
camera_pool_init(Camera_pool *pool)
{
  memset(&pool->entity_id[0], 0, sizeof(pool->entity_id));
  memset(&pool->camera[0], 0, sizeof(pool->camera));
  memset(&pool->peer_priority_00, 0, sizeof(pool->peer_priority_00));
  memset(&pool->peer_priority_01, 0, sizeof(pool->peer_priority_01));
  memset(&pool->peer_priority_02, 0, sizeof(pool->peer_priority_02));
  memset(&pool->peer_priority_03, 0, sizeof(pool->peer_priority_03));
}


void
camera_pool_add_camera(Camera_pool *pool,
                       const util::generic_id id,
                       const Camera::Camera_properties props)
{
  for(uint32_t i = 0; i < pool->number_of_cameras; ++i)
  {
    if(pool->entity_id[i] == util::generic_id_invalid())
    {
      pool->entity_id[i] = id;
      pool->camera[i] = props;
      return;
    }
  }
}


void
camera_pool_update_camera(Camera_pool *pool,
                          const util::generic_id id,
                          const Camera::Camera_properties props)
{
  for(uint32_t i = 0; i < pool->number_of_cameras; ++i)
  {
    if(pool->entity_id[i] == id)
    {
      pool->camera[i] = props;
      return;
    }
  }
}


void
camera_pool_remove_camera(Camera_pool *pool,
                          const util::generic_id id)
{
  // Find index of camera pool
  size_t index(0);
  if(util::generic_id_search_linearly(&index,
                                      id,
                                      pool->entity_id,
                                      pool->number_of_cameras))
  {
    pool->entity_id[index] = util::generic_id_invalid();
    pool->peer_priority_00[index] = 0;
    pool->peer_priority_01[index] = 0;
    pool->peer_priority_02[index] = 0;
    pool->peer_priority_03[index] = 0;
  }
}


void
camera_pool_set_priority(Camera_pool *pool,
                         const util::generic_id id,
                         const uint32_t peer,
                         const uint32_t priority)

{
  size_t index(0);
  if(util::generic_id_search_linearly(&index,
                                      id,
                                      pool->entity_id,
                                      pool->number_of_cameras))
  {
    switch(peer)
    {
      case(0): pool->peer_priority_00[index] = priority; break;
      case(1): pool->peer_priority_01[index] = priority; break;
      case(2): pool->peer_priority_02[index] = priority; break;
      case(3): pool->peer_priority_03[index] = priority; break;
    };
  }
}


uint32_t
camera_pool_get_priority(Camera_pool *pool,
                         const util::generic_id id,
                         const uint32_t peer)
{
  size_t index(0);
  if(util::generic_id_search_linearly(&index,
                                      id,
                                      pool->entity_id,
                                      pool->number_of_cameras))
  {
    switch(peer)
    {
      case(0): return pool->peer_priority_00[index]; break;
      case(1): return pool->peer_priority_01[index]; break;
      case(2): return pool->peer_priority_02[index]; break;
      case(3): return pool->peer_priority_03[index]; break;
    };
  }
  
  return 0;
}


Camera::Camera_properties
camera_pool_get_properties_for_priority(Camera_pool *pool,
                                        const uint32_t peer,
                                        const uint32_t priority,
                                        const uint32_t default_vp_width,
                                        const uint32_t default_vp_height)
{
  assert(pool);
  
  for(uint32_t i = 0; i < pool->number_of_cameras; ++i)
  {
    if(pool->peer_priority_00[i] == priority)
    {
      Camera::Camera_properties cam = pool->camera[i];
      
      if(!cam.viewport_height && !cam.viewport_width)
      {
        cam.viewport_width = default_vp_width;
        cam.viewport_height = default_vp_height;
      }
    
      return cam;
    }
  }
  
  // Failed
  return Camera::Camera_properties();
}


util::generic_id
camera_pool_get_entity_id_for_priority(Camera_pool *pool,
                                       const uint32_t peer,
                                       const uint32_t priority)
{
  assert(pool);

  // TODO: OMG!
  for (uint32_t i = 0; i < pool->number_of_cameras; ++i)
  {
    if (peer == 0 && pool->peer_priority_00[i] == priority)
    {
      return pool->entity_id[i];
    }
    if (peer == 1 && pool->peer_priority_01[i] == priority)
    {
      return pool->entity_id[i];
    }
    if (peer == 2 && pool->peer_priority_02[i] == priority)
    {
      return pool->entity_id[i];
    }
    if (peer == 3 && pool->peer_priority_03[i] == priority)
    {
      return pool->entity_id[i];
    }
  }

  // Failed
  return util::generic_id_invalid();
}


} // ns
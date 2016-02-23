#include "model.hpp"
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/model_pool.hpp>
#include <stddef.h>


namespace Core {


Model::Model(const uint32_t model_resource_id)
: m_model_id(model_resource_id)
{
}


math::aabb
Model::get_model_aabb() const
{
  math::aabb return_aabb;
  
  World_data::World *world = World_data::get_world();
  size_t id_index;
  
  if(world && math::index_linear_search(m_model_id,
                                        world->model_pool->id,
                                        world->model_pool->size,
                                        &id_index))
  {
    return_aabb = world->model_pool->aabb[id_index];
  }
  
  return return_aabb;
}

} // ns
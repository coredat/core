#include "logic_base.hpp"
#include <data/data.hpp>


namespace Logic {


math::transform
Base::get_transform() const
{
  if(world_data)
  {
    std::size_t index;
    if(Entity::find_index_linearly(&index, m_entity, world_data->entity_pool->entity_id, world_data->entity_pool->size))
    {
      return world_data->entity_pool->transform[index];
    }
  }
  
  return math::transform();
}


void
Base::set_transform(math::transform &trans)
{
  if(world_data)
  {
    std::size_t index;
    if(Entity::find_index_linearly(&index, m_entity, world_data->entity_pool->entity_id, world_data->entity_pool->size))
    {
      world_data->entity_pool->transform[index] = trans;
    }
  }
}


} // ns
#include "logic_base.hpp"
#include <data/data.hpp>


namespace Logic {


math::transform
Base::get_transform() const
{
  if(m_data_pool)
  {
    std::size_t index;
    if(Entity::find_index_linearly(&index, m_entity, m_data_pool->entity_id, m_data_pool->size))
    {
      return m_data_pool->transform[index];
    }
  }
  
  return math::transform();
}


void
Base::set_transform(math::transform &trans)
{
  if(m_data_pool)
  {
    std::size_t index;
    if(Entity::find_index_linearly(&index, m_entity, m_data_pool->entity_id, m_data_pool->size))
    {
      m_data_pool->transform[index] = trans;
    }
  }
}


} // ns
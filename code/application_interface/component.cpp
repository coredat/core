#include "component.hpp"
#include "entity.hpp"
#include <data/data.hpp>


namespace Core {


Core::Entity
Component::get_entity() const
{
  ::Core::Entity entity;
  Data::world_find_entity(m_world_data, &entity, m_entity_id);
  return entity;
}


} // ns
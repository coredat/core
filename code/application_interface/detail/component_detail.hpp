#ifndef COMPONENT_DETAIL_INCLUDED_BB59F5AE_E62F_426E_901A_EBDDB0F50CC3
#define COMPONENT_DETAIL_INCLUDED_BB59F5AE_E62F_426E_901A_EBDDB0F50CC3


#include "component.hpp"
#include <data/data_fwd.hpp>
#include <systems/entity/entity_id.hpp>


#define COMPONENT_MEMBERS                                    \
  ::Entity::Entity_id m_entity_id = ::Entity::invalid_id();  \
  Data::World *m_world_data;                                 \


#endif // inc gaurd
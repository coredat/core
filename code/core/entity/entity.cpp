#include "entity.hpp"
#include <data/world_data/world_data.hpp>


namespace Core {


Entity::Entity()
{
}


void
Entity::destroy()
{
  if(!is_valid()) { return; }
  
  // Destroy all children.
//  for(uint32_t c = 0; c < get_number_of_children(); ++c)
//  {
//    get_child(c).destroy();
//  }
  
  // Destroy this.
  World_data::entity_graph_change_push(m_world_data->entity_graph_changes, m_this_id, World_data::Entity_graph_change::removed);
}


} // ns
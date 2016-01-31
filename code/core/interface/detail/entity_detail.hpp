#ifndef ENTITY_DETAIL_INCLUDED_299D4604_85EE_465C_A900_CC774C05CF13
#define ENTITY_DETAIL_INCLUDED_299D4604_85EE_465C_A900_CC774C05CF13


#include <data/data_fwd.hpp>
#include <core/entity_id.hpp>


// This is used to allow the factory to make a public version
// of the object to set members. Keeping setters etc out of the
// objects interface.
#define ENTITY_MEMBERS                                                  \
  Core::Entity_id           m_this_id     = Core::entity_invalid_id();  \
  ::Data::World             *m_world_data = nullptr;                    \


namespace Core {
namespace Detail {
  
/*
  This is used to allow us to set the
  private member variables.
  
  We do this to keep Data::Entity's interface clean.
*/
struct Private_entity
{
  ENTITY_MEMBERS
};


inline void
set_entity_members(void *ent, ::Data::World *world_data, Core::Entity_id id)
{
  auto private_entity          = reinterpret_cast<Private_entity*>(ent);
  private_entity->m_world_data = world_data;
  private_entity->m_this_id    = id;
}
  

} // ns
} // ns


#endif // inc guard
#include <core/entity/entity_ref.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/detail/entity_id.hpp>


namespace Core {


Entity_ref::Entity_ref()
: Entity_ref(Core_detail::entity_id_invalid())
{
}


Entity_ref::Entity_ref(const Core_detail::Entity_id id)
: Entity_interface(id)
{
}


Entity_ref::Entity_ref(const Entity &entity)
{
  copy(entity);
}


Entity_ref::~Entity_ref()
{
  // Entity Ref does nothing, it is an observer here.
}


Entity_ref::Entity_ref(const Entity_ref &entity)
: Entity_interface(entity)
{
}


Entity_ref&
Entity_ref::operator=(const Entity_ref &entity)
{
  copy(entity);
  
  return *this;
}


} // ns
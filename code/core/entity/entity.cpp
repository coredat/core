#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>


namespace Core {


Entity::Entity()
{
}


Entity::Entity(Core::World &world)
: Entity_interface(world)
{
}


Entity::~Entity()
{
  if(is_valid())
  {
    destroy();
  }
}


Entity::Entity(Entity &&other)
{
  move(other);
}


Entity&
Entity::operator=(Entity &&other)
{
  move(other);
  
  return *this;
}


Entity::operator Entity_ref() const
{
  return Entity_ref(*const_cast<Entity*>(this));
}


} // ns
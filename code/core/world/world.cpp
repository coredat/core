#include "world.hpp"


namespace Core {


World::World(const World_setup &setup)
{
}


Entity
World::create_entity()
{
  return Entity();
}


Entity_ref
World::find_entity(const char *name)
{
  return Entity_ref();
}


} // ns
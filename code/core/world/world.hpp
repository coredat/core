#ifndef WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488
#define WORLD_INCLUDED_F6D825F0_D062_4B85_A636_7EDE94635488


#include "world_setup.hpp"
#include "entity.hpp"
#include "entity_ref.hpp"
#include <stdint.h>


namespace Core {


class World
{
public:

  explicit          World(const World_setup &setup);

  Entity            create_entity();
  Entity_ref        find_entity(const char *name);

};


} // ns


#endif // inc guard
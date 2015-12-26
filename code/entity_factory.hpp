#ifndef ENTITY_FACTORY_INCLUDED_
#define ENTITY_FACTORY_INCLUDED_


#include <data/entity/entity_fwd.hpp>


namespace Entity_factory {


void create_ground(Entity::Data *data);
void create_random_cube(Entity::Data *data);
void create_actor(Entity::Data *data);


} // ns


#endif // inc guard
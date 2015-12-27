#ifndef ENTITY_FACTORY_INCLUDED_
#define ENTITY_FACTORY_INCLUDED_


#include <data/entity/entity_fwd.hpp>


namespace Entity_factory {


Entity_id create_ground(Entity::Data *data);
Entity_id create_random_cube(Entity::Data *data);
Entity_id create_actor(Entity::Data *data);


} // ns


#endif // inc guard
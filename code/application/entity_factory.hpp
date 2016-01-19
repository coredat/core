#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <systems/entity/entity_fwd.hpp>
#include <data/data_fwd.hpp>


namespace Entity_factory {


Entity::Entity_id
create_ground(Data::World *world);


Entity::Entity_id
create_random_cube(Data::World *world);


Entity::Entity_id
create_actor(Data::World *world);


Entity::Entity_id
create_kinematic_actor(Data::World *world);


Entity::Entity_id
create_placement_cube(Data::World *world);


Entity::Entity_id
create_connection_node(Data::World *world);

} // ns


#endif // inc guard
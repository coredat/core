#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <systems/entity/entity_fwd.hpp>
#include <data/data_fwd.hpp>


namespace Entity_factory {


Data::Entity
create_ground(Data::World *world);


Data::Entity
create_random_cube(Data::World *world);


Data::Entity
create_actor(Data::World *world);


Data::Entity
create_kinematic_actor(Data::World *world);


Data::Entity
create_placement_cube(Data::World *world);


Data::Entity
create_connection_node(Data::World *world);

} // ns


#endif // inc guard
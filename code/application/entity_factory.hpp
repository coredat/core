#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <application_interface/application_interface_fwd.hpp>
#include <data/data_fwd.hpp>


namespace Entity_factory {


Core::Entity
create_ground(Data::World *world);


Core::Entity
create_random_cube(Data::World *world);


Core::Entity
create_actor(Data::World *world);


Core::Entity
create_local_kinematic_actor(Data::World *world);


Core::Entity
create_network_kinematic_actor(Data::World *world);


Core::Entity
create_placement_cube(Data::World *world);


Core::Entity
create_connection_node(Data::World *world);

} // ns


#endif // inc guard
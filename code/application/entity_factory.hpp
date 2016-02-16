#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <core/interface/interface_fwd.hpp>
#include <data/world_data/world_data_fwd.hpp>
#include <math/transform/transform.hpp>


namespace Entity_factory {


Core::Entity
create_ground(World_data::World *world);


Core::Entity
create_random_cube(World_data::World *world);


Core::Entity
create_local_kinematic_actor(World_data::World *world);


Core::Entity
create_network_kinematic_actor(World_data::World *world);


Core::Entity
create_npc_actor(World_data::World *world);


Core::Entity
create_placement_cube(World_data::World *world);


Core::Entity
create_static_cube(World_data::World *world, const math::transform &transform);

} // ns


#endif // inc guard
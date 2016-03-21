#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <core/interface/interface_fwd.hpp>
#include <core/transform/transform.hpp>
#include <data/world_data/world_data_fwd.hpp>

namespace Entity_factory {


Core::Entity
create_level(World_data::World *world);


Core::Entity
create_game_state(World_data::World *world);


Core::Entity
create_game_play_camera(World_data::World *world);


Core::Entity
create_bullet(World_data::World *world);


Core::Entity
create_actor(World_data::World *world);


Core::Entity
create_enemy(World_data::World *world);


Core::Entity
create_explosion(World_data::World *world, const Core::Transform transform);

} // ns


#endif // inc guard
#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <data/entity/entity_fwd.hpp>
#include <data/mesh_manager/mesh_data.hpp> // fwd dec
#include <data/texture_manager/texture_data.hpp> // fwd dec
#include <data/texture_pool.hpp>


namespace Entity_factory {


Entity::Entity_id
create_ground(Entity::Data *data, const Data::Mesh *mesh_resources, const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_random_cube(Entity::Data *data, const Data::Mesh *mesh_resources, const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_actor(Entity::Data *data, const Data::Mesh *mesh_resources, const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_kinematic_actor(Entity::Data *data, const Data::Mesh *mesh_resources, const Data::Texture_pool *texture_resources);


} // ns


#endif // inc guard
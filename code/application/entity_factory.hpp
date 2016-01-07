#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <systems/entity/entity_fwd.hpp>
#include <data/texture_pool.hpp> // fwd decs
#include <data/model_pool.hpp> // fwd decs
#include <data/entity_pool.hpp> // fwd decs


namespace Entity_factory {


Entity::Entity_id
create_ground(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_random_cube(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_actor(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_kinematic_actor(Data::Entity_pool *entity, const Data::Model_pool *mesh_resources, const Data::Texture_pool *texture_resources);


} // ns


#endif // inc guard
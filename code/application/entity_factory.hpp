#ifndef ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C
#define ENTITY_FACTORY_INCLUDED_465D023E_7584_4B89_8A1C_92294723073C


#include <systems/entity/entity_fwd.hpp>
#include <data/data_fwd.hpp>


namespace Entity_factory {


Entity::Entity_id
create_ground(const Entity::Entity_id parent,
              Data::Entity_pool *entity,
              Data::Pending_rigidbody_pool *pending_rb_pool,
              const Data::Model_pool *mesh_resources,
              const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_random_cube(const Entity::Entity_id parent,
                   Data::Entity_pool *entity,
                   Data::Pending_rigidbody_pool *pending_rb_pool,
                   const Data::Model_pool *mesh_resources,
                   const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_actor(const Entity::Entity_id parent,
             Data::Entity_pool *entity,
             Data::Pending_rigidbody_pool *pending_rb_pool,
             const Data::Model_pool *mesh_resources,
             const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_kinematic_actor(const Entity::Entity_id parent,
                       Data::Entity_pool *entity,
                       Data::Pending_rigidbody_pool *pending_rb_pool,
                       const Data::Model_pool *mesh_resources,
                       const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_placement_cube(const Entity::Entity_id parent,
                      Data::Entity_pool *entity,
                      Data::Pending_rigidbody_pool *pending_rb_pool,
                      const Data::Model_pool *mesh_resources,
                      const Data::Texture_pool *texture_resources);


Entity::Entity_id
create_connection_node(const Entity::Entity_id parent,
                       Data::Entity_pool *entity,
                       Data::Pending_rigidbody_pool *pending_rb_pool,
                       const Data::Model_pool *mesh_resources,
                       const Data::Texture_pool *texture_resources);

} // ns


#endif // inc guard